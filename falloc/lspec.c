
# define        _XOPEN_SOURCE       600

# include       <assert.h>
# include       <fcntl.h>
# include       <stdlib.h>
# include       <stdio.h>
# include       <unistd.h>
# include       <string.h>
# include       <ctype.h>
# include       <sys/types.h>
# include       <sys/stat.h>
# include       <errno.h>


extern void fatal(char *str);


off_t lengthSpec(char *str) {
    /* suffix multipliers in the style of dd so we don't reinvent that wheel */
    char *errptr;
    off_t size = 0, mult = 0;
    int i = strlen(str);
    char *pp;

    /* look for trailing multiplier suffixes - single char first... */
    switch(str[i - 1]) {
      case 'c': mult = 1l;                      break;
      case 'w': mult = 2l;                      break;
      case 'b': mult = 512l;                    break;
      case 'K': mult = 1024l;                   break;
      case 'M': mult = 1024l * 1024l;           break;
      case 'G': mult = 1024l * 1024l * 1024l;   break;
    }

    if(mult != 0)
        str[i - 1] = '\0';
    /* or two-character suffixes... */
    else if(str[i - 1] == 'B') {
        switch(str[i - 2]) {
          case 'k': mult = 1000l;                   break;
          case 'M': mult = 1000l * 1000l;           break;
          case 'G': mult = 1000l * 1000l * 1000l;   break;
        }

        if(mult != 0)
            str[i - 2] = '\0';
    }
    /* and finally an optionally sized page */
    else if((pp = strchr(str, 'p'))) {
        if(! pp[1])
            /* if not followed by digits, assume default page size 2K  */
            /* TODO: if porting to Windows, then consider strongly the */
            /* need to assume the 4K default size used by IDS on NTFS  */
            mult = 2048l;
        else {
            for(i = 1; isdigit(pp[i]); ++i)
                mult = 10 * mult + pp[i] - '0';

            /* complain if the p## suffix isn't clean */
            /* piggy-back the complaint for strtoll   */
            if(pp[i]) {
                errno = EINVAL;
                fatal("strtoll");
            }

            /* turn the mult into K */
            mult *= 1024l;
        }

        /* chop off the p## suffix */
        *pp = '\0';
    }

    if(mult == 0)
        mult = 1;
    assert(mult > 0);

    errno = 0;
    size = strtoll(str, &errptr, 10);
    if(! *str || *errptr) {
        if(! errno) errno = EINVAL;
        fatal("strtoll");
    }
    size *= mult;
    return size;
}   /* lengthSpec */

