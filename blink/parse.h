
typedef enum
{  
   EOK = 0,
   E01,         // too long information 
   E02,         // bad header "drv_" on front side string  
   E03,         // bad value for first argument 
   E04,
   E05

}PARSE_ERRORS;

PARSE_ERRORS ParseData(CIRC_BUFF *circ_buf);