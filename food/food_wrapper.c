// this is launchproxy.c with a few modifications
/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_APACHE_LICENSE_HEADER_START@
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @APPLE_APACHE_LICENSE_HEADER_END@
 */
#include <sys/types.h>
#include <sys/select.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <libgen.h>
#include <getopt.h>
#include <signal.h>
#include <netdb.h>
#undef __BLOCKS__
#include <notify.h>

#include "launch.h"

// Use darwin notifications.
// Before we actually launch, send a fake message for the name of the magic notification
// Eewwww... but it will work

const char *prog;

static int kq = 0;

static void find_fds(launch_data_t o, const char *key __attribute__((unused)), void *context __attribute__((unused)))
{
	struct kevent kev;
	size_t i;
	int fd;

	switch (launch_data_get_type(o)) {
	case LAUNCH_DATA_FD:
		fd = launch_data_get_fd(o);
		if (-1 == fd)
			break;
		fcntl(fd, F_SETFD, 1);
		EV_SET(&kev, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(kq, &kev, 1, NULL, 0, NULL) == -1)
			syslog(LOG_DEBUG, "kevent(%d): %m", fd);
		break;
	case LAUNCH_DATA_ARRAY:
		for (i = 0; i < launch_data_array_get_count(o); i++)
			find_fds(launch_data_array_get_index(o, i), NULL, NULL);
		break;
	case LAUNCH_DATA_DICTIONARY:
		launch_data_dict_iterate(o, find_fds, NULL);
		break;
	default:
		break;
	}
}

int main(int argc __attribute__((unused)), char *argv[])
{
	struct timespec timeout = { 10, 0 };
	struct sockaddr_storage ss;
	socklen_t slen = (socklen_t)sizeof ss;
	struct kevent kev;
	int r, ec = EXIT_FAILURE;
	launch_data_t tmp, resp, msg = launch_data_alloc(LAUNCH_DATA_STRING);
    prog = argv[1];
	bool w = false, dupstdout = false, dupstderr = false;

	launch_data_set_string(msg, LAUNCH_KEY_CHECKIN);

	openlog(getprogname(), LOG_PERROR|LOG_PID|LOG_CONS, LOG_LAUNCHD);

	kq = kqueue();

	if ((resp = launch_msg(msg)) == NULL) {
		syslog(LOG_ERR, "launch_msg(%s): %m", LAUNCH_KEY_CHECKIN);
		goto out;
	}

	launch_data_free(msg);

	tmp = launch_data_dict_lookup(resp, LAUNCH_JOBKEY_SOCKETS);
	if (tmp) {
		find_fds(tmp, NULL, NULL);
	} else {
		syslog(LOG_ERR, "No FDs found to answer requests on!");
		goto out;
	}

	tmp = launch_data_dict_lookup(resp, LAUNCH_JOBKEY_TIMEOUT);
	if (tmp)
		timeout.tv_sec = (int)launch_data_get_integer(tmp);

	tmp = launch_data_dict_lookup(resp, LAUNCH_JOBKEY_PROGRAM);
	if (tmp)
		prog = launch_data_get_string(tmp);

	w = false;

	if (launch_data_dict_lookup(resp, LAUNCH_JOBKEY_STANDARDOUTPATH))
		dupstdout = false;

	if (launch_data_dict_lookup(resp, LAUNCH_JOBKEY_STANDARDERRORPATH))
		dupstderr = false;

	if (!w)
		signal(SIGCHLD, SIG_IGN);

	for (;;) {
		if ((r = kevent(kq, NULL, 0, &kev, 1, &timeout)) == -1) {
			syslog(LOG_DEBUG, "kevent(): %m");
			goto out;
		} else if (r == 0) {
			ec = EXIT_SUCCESS;
			goto out;
		}

        if(kev.udata != NULL) {
            syslog(LOG_WARNING, "got a kev with udata %d", (int) kev.udata);
            close((int)kev.ident);
            kill((pid_t)kev.udata, SIGKILL);
            continue;
        }

		if (w) {
			dup2((int)kev.ident, STDIN_FILENO);
			if (dupstdout)
				dup2((int)kev.ident, STDOUT_FILENO);
			if (dupstderr)
				dup2((int)kev.ident, STDERR_FILENO);
			execv(prog, argv + 1);
			syslog(LOG_ERR, "execv(): %m");
			exit(EXIT_FAILURE);
		}

		if ((r = accept((int)kev.ident, (struct sockaddr *)&ss, &slen)) == -1) {
			if (errno == EWOULDBLOCK)
				continue;
			syslog(LOG_WARNING, "accept(): %m");
			goto out;
		} else {
			if (ss.ss_family == AF_INET || ss.ss_family == AF_INET6) {
				char fromhost[NI_MAXHOST];
				char fromport[NI_MAXSERV];
				int gni_r;

				gni_r = getnameinfo((struct sockaddr *)&ss, slen,
						fromhost, (socklen_t) sizeof fromhost,
						fromport, (socklen_t) sizeof fromport,
						NI_NUMERICHOST | NI_NUMERICSERV);

				if (gni_r) {
					syslog(LOG_WARNING, "%s: getnameinfo(): %s", prog, gai_strerror(gni_r));
				} else {
					syslog(LOG_INFO, "%s: Connection from: %s on port: %s", prog, fromhost, fromport);
				}
			} else {
				syslog(LOG_WARNING, "%s: getnameinfo() only supports IPv4/IPv6. Connection from address family: %u", prog, ss.ss_family);
			}

            char *sekrit;
            asprintf(&sekrit, "com.%08x%08x.die.die.die", arc4random(), arc4random());

            int notify_fd, rtoken;
            if(notify_register_file_descriptor(sekrit, &notify_fd, 0, &rtoken) != NOTIFY_STATUS_OK) {
                syslog(LOG_WARNING, "%s: notify_register_file_descriptor error", prog);
            }

            size_t msgsize;
            struct set_sekrit_req {
                int magic;
                size_t msgsize;
                int is_resp;
                int msgid;
                int funcid;
                size_t sekrit_len;
                char sekrit[];
            } *req = malloc(msgsize = sizeof(*req) + strlen(sekrit));
            req->magic = 0x12345678;
            req->msgsize = msgsize;
            req->is_resp = 0;
            req->msgid = -1;
            req->funcid = 1;
            req->sekrit_len = strlen(sekrit);
            memcpy(req->sekrit, sekrit, strlen(sekrit));
            send(r, req, msgsize, 0);
            free(req);
            free(sekrit);

            pid_t pid = fork();
			switch (pid) {
			case -1:
				syslog(LOG_WARNING, "fork(): %m");
				if( errno != ENOMEM ) {
					continue;
				}
				goto out;
			case 0:
				break;
			default:
                {
                struct kevent kev_;
                EV_SET(&kev_, notify_fd, EVFILT_READ, EV_ADD, 0, 0, (void *) pid);
                if(kevent(kq, &kev_, 1, NULL, 0, NULL) == -1) {
                    syslog(LOG_WARNING, "%s: kevent error", prog);
                }

				close(r);
				continue;
                }
			}


			setpgid(0, 0);

			fcntl(r, F_SETFL, 0);
			fcntl(r, F_SETFD, 1);
            dup2(r, 3);
			signal(SIGCHLD, SIG_DFL);
			execv(prog, argv + 1);
			syslog(LOG_ERR, "execv(): %m");
			exit(EXIT_FAILURE);
		}
	}

out:
	exit(ec);
}

