#ifndef LIBUBOOTENV_H
#define LIBUBOOTENV_H

void libubootenv_spy_init();
void libubootenv_spy_deinit();
void libubootenv_spy_push_iter(const char* in_key, const char* in_val);

#endif /* LIBUBOOTENV_H */
