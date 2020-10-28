# ft_ping

## ICMP Structure
### netinet/ip_icmp.h
```C
struct icmp {
 u_char	icmp_type;		/* type of message, see below */
 u_char	icmp_code;		/* type sub code */
 u_short	icmp_cksum;		/* ones complement cksum of struct */
 union {
 	u_char ih_pptr;			/* ICMP_PARAMPROB */
 	struct in_addr ih_gwaddr;	/* ICMP_REDIRECT */
 	struct ih_idseq {
 		n_short	icd_id;
 		n_short	icd_seq;
 	} ih_idseq;
 	int ih_void;
 } icmp_hun;
```

### Stackoverflow
```C
typedef struct _icmphdr
{
    BYTE   i_type;
    BYTE   i_code;                 // Type sub code
    USHORT i_cksum;
    USHORT i_id;
    USHORT i_seq;
    // This is not the standard header, but we reserve space for time
    ULONG  timestamp;
} IcmpHeader;
```

### [imcp_hdr](https://docs.huihoo.com/doxygen/linux/kernel/3.7/uapi_2linux_2icmp_8h_source.html)
```C
struct icmphdr {
  __u8      type;
  __u8      code;
  __sum16   checksum;
  union {
    struct {
        __be16  id;
        __be16  sequence;
    } echo;
    __be32  gateway;
    struct {
        __be16  __unused;
        __be16  mtu;
    } frag;
  } un;
}
```
