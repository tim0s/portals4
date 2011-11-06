#include "config.h"

#include "portals4.h"
#include "ppe_if.h" 
        
#include "shared/ptl_internal_handles.h"
#include "ptl_internal_error.h"
#include "ptl_internal_nit.h"
#include "ptl_internal_EQ.h"

#define PT_FREE     0
#define PT_ENABLED  1
#define PT_DISABLED 2

int PtlPTAlloc(ptl_handle_ni_t ni_handle,
               unsigned int    options,
               ptl_handle_eq_t eq_handle,
               ptl_pt_index_t  pt_index_req,
               ptl_pt_index_t *pt_index)
{
    const ptl_internal_handle_converter_t ni = { ni_handle };

#ifndef NO_ARG_VALIDATION
    if (PtlInternalLibraryInitialized() == PTL_FAIL) {
        return PTL_NO_INIT;
    }
    if ((ni.s.ni >= 4) || (ni.s.code != 0) || (nit.refcount[ni.s.ni] == 0)) {
        VERBOSE_ERROR("Invalid NI passed to PtlPTAlloc\n");
        return PTL_ARG_INVALID;
    }
    if (options & ~PTL_PT_ALLOC_OPTIONS_MASK) {
        VERBOSE_ERROR("Invalid options to PtlPTAlloc (0x%x)\n", options);
        return PTL_ARG_INVALID;
    }
    if ((eq_handle == PTL_EQ_NONE) && options & PTL_PT_FLOWCTRL) {
        return PTL_PT_EQ_NEEDED;
    }
    if (PtlInternalEQHandleValidator(eq_handle, 1)) {
        VERBOSE_ERROR("Invalid EQ passed to PtlPTAlloc\n");
        return PTL_ARG_INVALID;
    }
    if ((pt_index_req > nit_limits[ni.s.ni].max_pt_index) && (pt_index_req != PTL_PT_ANY)) {
        VERBOSE_ERROR("Invalid pt_index request passed to PtlPTAlloc\n");
        return PTL_ARG_INVALID;
    }
    if (pt_index == NULL) {
        VERBOSE_ERROR("Invalid pt_index pointer (NULL) passed to PtlPTAlloc\n");
        return PTL_ARG_INVALID;
    }
#endif /* ifndef NO_ARG_VALIDATION */

    return PTL_OK;
}

int PtlPTFree(ptl_handle_ni_t ni_handle,
              ptl_pt_index_t  pt_index)
{
    const ptl_internal_handle_converter_t ni = { ni_handle };

#ifndef NO_ARG_VALIDATION
    if (PtlInternalLibraryInitialized() == PTL_FAIL) {
        VERBOSE_ERROR("Not initialized\n");
        return PTL_NO_INIT;
    }
    if ((ni.s.ni >= 4) || (ni.s.code != 0) || (nit.refcount[ni.s.ni] == 0)) {
        VERBOSE_ERROR
            ("ni.s.ni too big (%u >= 4) or ni.s.code wrong (%u != 0) or nit not initialized\n",
            ni.s.ni, ni.s.code);
        return PTL_ARG_INVALID;
    }
    if (pt_index == PTL_PT_ANY) {
        VERBOSE_ERROR("pt_index is PTL_PT_ANY\n");
        return PTL_ARG_INVALID;
    }
    if (pt_index > nit_limits[ni.s.ni].max_pt_index) {
        VERBOSE_ERROR("pt_index is too big (%u > %u)\n", pt_index,
                      nit_limits[ni.s.ni].max_pt_index);
        return PTL_ARG_INVALID;
    }
#endif /* ifndef NO_ARG_VALIDATION */

    return PTL_OK;
}


int PtlPTDisable(ptl_handle_ni_t ni_handle,
                 ptl_pt_index_t  pt_index)
{
    fprintf(stderr, "PtlPTDisable() unimplemented\n");
    return PTL_FAIL;

}


int PtlPTEnable(ptl_handle_ni_t ni_handle,
                ptl_pt_index_t  pt_index)

{
    fprintf(stderr, "PtlPTEnable() unimplemented\n");
    return PTL_FAIL;
}

int INTERNAL PtlInternalPTValidate(ptl_table_entry_t *t)
{
#ifndef NO_ARG_VALIDATION
    if (PtlInternalEQHandleValidator(t->EQ, 1)) {
        VERBOSE_ERROR("PTValidate sees invalid EQ handle\n");
        return 3;
    }
#endif
    switch (t->status) {
        case PT_FREE:
            VERBOSE_ERROR("PT has not been allocated\n");
            return 1;

        case PT_DISABLED:
            VERBOSE_ERROR("PT has been disabled\n");
            return 2;

        case PT_ENABLED:
            return 0;

        default:
            // should never happen
            *(int *)0 = 0;
            return 3;
    }
} 
