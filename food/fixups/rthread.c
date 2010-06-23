#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef pthread_attr_t *rthread_attr_t;
typedef pthread_cond_t *rthread_cond_t;
typedef pthread_condattr_t *rthread_condattr_t;
typedef pthread_mutex_t *rthread_mutex_t;
typedef pthread_mutexattr_t *rthread_mutexattr_t;
typedef pthread_t *rthread_t;

typedef pthread_key_t rthread_key_t;

pthread_key_t rthread_key;
__attribute__((constructor))
void init_rthread_key() {
    assert(!pthread_key_create(&rthread_key, NULL));
}
rthread_t rthread_self(void) {
    rthread_t ret = pthread_getspecific(rthread_key);
    if(!ret) {
        ret = calloc(1, sizeof(pthread_t));
        *ret = pthread_self();
        assert(!pthread_setspecific(rthread_key, ret));
    }
    return ret;
}

int       rthread_cond_signal(rthread_cond_t *cond) {
    return pthread_cond_signal(*cond);
}
int       rthread_mutexattr_destroy(rthread_mutexattr_t *attr) {
    return pthread_mutexattr_destroy(*attr); // leak
}
int       rthread_mutex_init(rthread_mutex_t *m, const rthread_mutexattr_t *ma) {
    *m = calloc(1, sizeof(pthread_mutex_t));
    //fprintf(stderr, "rthread_mutex_init %p with attributes %p\n", m, ma);
    return pthread_mutex_init(*m, ma == NULL ? NULL : *ma);
}
int       rthread_create(rthread_t *a,
                         const rthread_attr_t *b,
                         void *(*c)(void *),
                         void *d) {
    *a = calloc(1, sizeof(pthread_t));
    return pthread_create(*a, b ? *b : NULL, c, d);
}
int       rthread_cond_broadcast(rthread_cond_t *cond) {
    return pthread_cond_broadcast(*cond);
}
int       rthread_cond_destroy(rthread_cond_t *cond) {
    return pthread_cond_destroy(*cond); // leak
}
int       rthread_key_create(rthread_key_t *a, void (*b)(void *)) {
    return pthread_key_create(a, b);
}
int       rthread_attr_setstackaddr(rthread_attr_t *a,
                                      void *b) {
    return pthread_attr_setstackaddr(*a, b);
}
int       rthread_attr_setstacksize(rthread_attr_t *a, size_t b) {
    return pthread_attr_setstacksize(*a, b);
}
int rthread_getattr_np(rthread_t thread, rthread_attr_t *attr) {
    fprintf(stderr, "rthread_getattr_np called...\n");
    rthread_attr_setstackaddr(attr, pthread_get_stackaddr_np(*thread));
    rthread_attr_setstacksize(attr, pthread_get_stacksize_np(*thread));
    return 0;
    // It will then call getstack.
}

int       rthread_key_delete(rthread_key_t key) {
    return pthread_key_delete(key);
}
int       rthread_attr_init(rthread_attr_t *attr) {
     *attr = calloc(1, sizeof(pthread_attr_t));
    return pthread_attr_init(*attr);
}
void     *rthread_getspecific(rthread_key_t k) {
    return pthread_getspecific(k);
}

int       rthread_cond_init(rthread_cond_t *a,
                            const rthread_condattr_t *b) {
    *a = calloc(1, sizeof(pthread_cond_t));
    return pthread_cond_init(*a, b == NULL ? NULL : *b);
}
int       rthread_mutex_lock(rthread_mutex_t *a) {
    //fprintf(stderr, "lock %p\n", a);
    return pthread_mutex_lock(*a);
}
int       rthread_mutex_unlock(rthread_mutex_t *a) {
    //fprintf(stderr, "unlock %p\n", a);
    return pthread_mutex_unlock(*a);
}
int       rthread_mutex_trylock(rthread_mutex_t *a) {
    //fprintf(stderr, "trylock %p\n", a);
    return pthread_mutex_trylock(*a);
}
int       rthread_attr_setdetachstate(rthread_attr_t *a,
				      int b) {
    return pthread_attr_setdetachstate(*a, b);
}
int       rthread_mutex_destroy(rthread_mutex_t *a) {
    return pthread_mutex_destroy(*a);
}
int       rthread_cond_wait(rthread_cond_t *a,
			    rthread_mutex_t *b) {
    return pthread_cond_wait(*a, *b);
}
int       rthread_cond_timedwait(rthread_cond_t *a,
				 rthread_mutex_t *b,
				 const struct timespec *c) {
    fprintf(stderr, "timedwait: timespec=%u,%u\n", (unsigned int) c->tv_sec, (unsigned int) c->tv_nsec);
    return pthread_cond_timedwait(*a, *b, c);
}
int       rthread_mutexattr_settype(rthread_mutexattr_t *a, int b) {
    return pthread_mutexattr_settype(*a, b);
}
int       rthread_join(rthread_t a, void **b) {
    return pthread_join(*a, b);
}
int       rthread_setspecific(rthread_key_t a,
			      const void *b) {
    return pthread_setspecific(a, b);
}
int       rthread_attr_destroy(rthread_attr_t *a) {
    return pthread_attr_destroy(*a); // leak
}
int       rthread_attr_getstack(const rthread_attr_t *a,
                                      void **b, size_t *c) {
    return pthread_attr_getstack(*a, b, c);
}
int       rthread_mutexattr_init(rthread_mutexattr_t *a) {
    *a = calloc(1, sizeof(pthread_mutexattr_t));
    return pthread_mutexattr_init(*a);
}
