#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
#include "cbuffer.h"

#ifndef MAP_ANONYMOUS
#  define MAP_ANONYMOUS MAP_ANON
#endif

static char __create_buffer_mirror(cbuf_t* cb)
{
  char path[] = "/tmp/cb-XXXXXX";
  int fd;
  int	status;
  void *address;

  fd = mkstemp(path);
  check(fd != -1, "Create_mirror");
  status = unlink(path);
  check(!status, "_Create mirror");
  status = ftruncate(fd, cb->size);
  /* create the array of data */
  cb->data = mmap(NULL, cb->size << 1, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  check(cb->data != MAP_FAILED, "Create mirror");
  address = mmap(cb->data, cb->size, PROT_READ | PROT_WRITE,
		 MAP_FIXED | MAP_SHARED, fd, 0);
  check(address == cb->data, "Create mirror");
  address = mmap(cb->data + cb->size, cb->size, PROT_READ | PROT_WRITE,
		 MAP_FIXED | MAP_SHARED, fd, 0);
  check(address == (cb->data + cb->size), "Create mirror");
  status = close(fd);
  check(!status, "Create mirror");
  return (EXIT_SUCCESS);
 error:
  return (EXIT_FAILURE);
}

cbuf_t *new_circular_buffer(const unsigned int order)
{
  char		s;
  cbuf_t	*this;

  this = malloc(sizeof(cbuf_t));
  check_mem(this);
  this->size = 1UL << order;
  this->head = this->tail = 0;
  s = __create_buffer_mirror(this);
  check(s == EXIT_SUCCESS, "new_circular_buffer");
  return (this);
 error:
  return(NULL);
}

void cbuf_free(cbuf_t *this)
{
  if (this)
    {
      munmap(this->data, this->size << 1);
      free(this);
      this = NULL;
    }
  return ;
}

int cbuf_is_empty(const cbuf_t *this)
{
  return (this->head == this->tail);
}

int cbuf_write(cbuf_t *this, const unsigned char *data, const int size)
{
  int written;

  /* prevent buffer from getting completely full or over commited */
  check(cbuf_unusedspace(this) >= size && data && size > 0, "cbuf_write");
  written = cbuf_unusedspace(this);
  written = size < written ? size : written;
  memcpy(this->data + this->tail, data, written);
  this->tail += written;
  if (this->size < this->tail)
    this->tail %= this->size;
  return written;
 error:
  return(0);
}

unsigned char *cbuf_peek(const cbuf_t *this)
{
  if (cbuf_is_empty(this))
    return (NULL);
  return (this->data + this->head);
}

unsigned char *cbuf_read(cbuf_t *this, const unsigned int size)
{
  void *end;

  if (cbuf_is_empty(this))
    return (NULL);
  end = this->data + this->head;
  this->head += size;
  return (end);
}

int cbuf_size(const cbuf_t *this)
{
  return (this->size);
}

int cbuf_usedspace(const cbuf_t *this)
{
  if (this->head <= this->tail)
    return (this->tail - this->head);
  else
    return( this->size - (this->head - this->tail));
}

int cbuf_freespace(const cbuf_t *this)
{
  return (this->size - cbuf_usedspace(this));
}
