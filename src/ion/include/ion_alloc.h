#ifndef _ION_ALLOC_H
#define _ION_ALLOC_H

	   
#ifdef __cplusplus
extern "C"
{
#endif

extern int ion_alloc_open(void);
extern void ion_alloc_close(void);
extern void *ion_alloc_alloc(int size);
extern void ion_alloc_free(void *ptr);
extern void *ion_alloc_vir2phy(void *ptr);
extern void *ion_alloc_phy2vir(void *ptr);
extern void ion_flush_cache(void *startAddr, int size);

   
#ifdef __cplusplus
}
#endif

#endif /* _ION_ALLOC_H */
