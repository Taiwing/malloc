/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yforeau <yforeau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 12:30:33 by yforeau           #+#    #+#             */
/*   Updated: 2022/06/09 22:05:40 by yforeau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_H
# define NETWORK_H

# include <netinet/ip.h>
# include <linux/ipv6.h>
# include <netinet/tcp.h>
# include <netinet/udp.h>
# include <netinet/ip_icmp.h>
# include <linux/icmpv6.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <linux/filter.h>
# include <netinet/if_ether.h>
# include <linux/if.h>

# define	BPF_FILTER_SIZE(arr)		(sizeof(arr) / sizeof(arr[0]))

/*
** IP union (better than an unIP union I guess... ROFL) for v4 and v6
*/

typedef union			u_ip
{
	uint16_t			family;
	struct sockaddr_in	v4;
	struct sockaddr_in6	v6;
}						t_ip;

/*
** Reply header types and unions
*/

enum e_iphdr	{ E_IH_NONE = 0, E_IH_V4, E_IH_V6 };
enum e_nexthdr	{ E_NH_NONE = 0, E_NH_ICMP, E_NH_ICMP6, E_NH_TCP, E_NH_UDP };

// The first header
typedef union		u_iphdr
{
	struct iphdr	v4;
	struct ipv6hdr	v6;
}					t_iphdr;

// The second header
typedef union		u_nexthdr
{
	struct tcphdr	tcp;
	struct udphdr	udp;
	struct icmphdr	icmp;
	struct icmp6hdr	icmp6;
}					t_nexthdr;

# define MAX_PACKET_PAYLOAD_SIZE	512
# define MAX_RAW_DATA_SIZE	\
	((sizeof(t_iphdr) + sizeof(t_nexthdr)) * 2 + MAX_PACKET_PAYLOAD_SIZE)

/*
** t_packet: IPv4/IPv6 + [ICMP/ICMPv6] + [IPv4/IPv6] + [UDP/TCP] + [payload]
**
** iphdr: type of the first ip header
** nexthdr: type of the next header
** nextiphdr: type of the second ip header
** lasthdr: type of nextiphdr's next header
** ip: pointer to ip header if not NONE
** next: pointer to next header
** nextip: pointer to second ip header if not NONE
** last: pointer to next header if any
** size: size of raw data (every headers)
** raw_data: where every headers are stored
** buf: static buffer if the storage is not extern
*/

typedef struct		s_packet
{
	enum e_iphdr	iphdr;
	enum e_nexthdr	nexthdr;
	enum e_iphdr	nextiphdr;
	enum e_nexthdr	lasthdr;
	t_iphdr			*ip;
	t_nexthdr		*next;
	t_iphdr			*nextip;
	t_nexthdr		*last;
	size_t			size;
	uint8_t			*raw_data;
	uint8_t			buf[MAX_RAW_DATA_SIZE];
}					t_packet;

# define	MIN_HEADER_LEVEL	0
# define	MAX_HEADER_LEVEL	4

/*
** Arguments for header initialization functions
*/

typedef struct		s_iph_args
{
	uint8_t			version;
	t_ip			*dstip;
	t_ip			*srcip;
	uint16_t		protocol;
	uint8_t			hop_limit;
	uint16_t		layer5_len;
}					t_iph_args;

typedef struct		s_tcph_args
{
	void			*iphdr;
	uint8_t			version;
	uint16_t		srcp;
	uint16_t		dstp;
	uint32_t		seq;
	uint32_t		ack;
	uint8_t			flags;
	uint16_t		win;
	uint16_t		urp;
}					t_tcph_args;

/*
** t_filter_spec: libft packet filter structure
**
** This structure can be passed to every 'ft_packet_filter' function. All the
** fields are not necessary for every function but src must not be NULL because
** it is absolutely needed for host filtering and to know the IP family on which
** to filter (except for ftscan filters). Also if both src and dst are set their
** family value must be the same otherwise the filtering functions will fail.
** For port ranges, if needed by the filter function, max must always be greater
** or equal to min port.
**
** protocol: IP header protocol/next_header field (UDP, TCP, ICMP or ICMPv6)
** src: address of the host to receive packets from
** dst: local interface address
** icmp_type: ICMP/ICMPv6 type field
** icmp_code: ICMP/ICMPv6 code field
** icmp_echo_id: ICMP/ICMPv6 echo id field
** icmp_echo_sequence: ICMP/ICMPv6 echo sequence field
** icmp_protocol: protocol field in the ICMP payload (UDP or TCP)
** min_src_port: smallest src TCP/UDP port in ICMP payload port range
** max_src_port: biggest src TCP/UDP port in ICMP payload port range
** min_dst_port: smallest dst TCP/UDP port in ICMP payload port range
** max_dst_port: biggest dst TCP/UDP port in ICMP payload port range
*/

typedef struct		s_filter_spec
{
	uint8_t			protocol;
	t_ip			*src;
	t_ip			*dst;
	uint8_t			icmp_type;
	uint8_t			icmp_code;
	uint16_t		icmp_echo_id;
	uint16_t		icmp_echo_sequence;
	uint8_t			icmp_protocol;
	uint16_t		min_src_port;
	uint16_t		max_src_port;
	uint16_t		min_dst_port;
	uint16_t		max_dst_port;
}					t_filter_spec;

// ft_ip_rand flags
enum e_ip_rand_flags {
	E_IPRAND_ALLOW_LOCAL	= 1 << 0,	// Allow local IP generation
	E_IPRAND_RANDOM_DOMAIN	= 1 << 1,	// Randomize the domain if AF_UNSPEC
};

/*
** IP utility functions
*/

size_t		ft_ip_sock_size(const t_ip *ip);
size_t		ft_ip_size(const t_ip *ip);
uint8_t		*ft_ip_addr(const t_ip *ip);
void		ft_ip_set_port(t_ip *ip, uint16_t port);
int			ft_ip_cmp(const t_ip *a, const t_ip *b);
int			ft_ip_apply_mask(t_ip *dest, const t_ip *mask);
int			ft_ip_same_subnet(const t_ip *a, const t_ip *b, const t_ip *mask);
int			ft_get_ip(t_ip *ip, const char *target, int domain);
int			ft_ip_rand(t_ip *ip, size_t count, int domain, int flags);
char		*ft_ip_str(const t_ip *ip);

/*
** IP headers
*/

void		ft_init_ip_header(void *ipptr, t_iph_args *args);

/*
** Layer 4 Headers
*/

uint32_t	ft_sum_bit16(uint16_t *data, size_t sz);
uint16_t	ft_checksum(uint16_t *data, size_t sz);
int			ft_transport_checksum(int version, void *iphdr,
				uint8_t *packet, uint16_t len);
void		ft_init_udp_header(uint8_t *udp_packet, void *iphdr,
				uint16_t srcp, uint16_t dstp);
void		ft_init_tcp_header(uint8_t *tcp_packet, t_tcph_args *args);

/*
** Print Headers
*/

int			ft_print_iphdr(void *iphdr, int domain, char *exec);
int			ft_print_nexthdr(void *iphdr, int domain,
				uint16_t size, char *exec);
int			ft_print_icmphdr(void *icmph, int domain,
				uint16_t size, char *exec);
void		ft_print_udphdr(struct udphdr *udph);
void		ft_print_tcphdr(struct tcphdr *tcph);
int			ft_print_packet(void *packet, int domain, size_t size, char *exec);

/*
** Send and recv sockets (simply for documentation purposes)
*/

typedef int	t_send_socket;
typedef int	t_recv_socket;

/*
** socket initialization functions
*/

t_send_socket	ft_send_socket_init(int domain, int protocol, int include_ip);
t_recv_socket	ft_recv_socket_init(int domain);

/*
** Packet functions
*/

size_t		ft_iphdr_size(enum e_iphdr iphdr);
size_t		ft_nexthdr_size(enum e_nexthdr nexthdr);
void		ft_packet_reset(t_packet *packet, uint8_t *datap);
void		ft_packet_init(t_packet *packet, enum e_iphdr iph, uint8_t *datap);
int			ft_packet_filter_layer4(t_recv_socket recvfd, t_filter_spec *spec);
int			ft_packet_filter_icmp_layer4(t_recv_socket recvfd,
				t_filter_spec *spec);
int			ft_packet_send(t_send_socket sendfd, t_ip *dst,
				t_packet *packet, int level);


/*
** Network config
*/

/*
** t_ifinfo: Interface info
**
** name: interface name
** flags: interface flags from getifaddrs
** ip: interface socket address
** netmask: interface netmask
*/
typedef struct		s_ifinfo
{
	char			name[IFNAMSIZ];
	uint32_t		flags;
	t_ip			ip;
	t_ip			netmask;
}					t_ifinfo;

/*
** Network config functions
*/

int	ft_net_getiface(t_ifinfo *dst, int domain, int loopback);
int	ft_net_setiface(int loopback, const t_ifinfo *src);

/*
** Scan structures
*/

// Scan types
enum e_ftscan_type	{ E_FTSCAN_ECHO_PING, E_FTSCAN_TCP_SYN };

# define			FTSCAN_TYPE_COUNT			2
# define			DEF_FTSCAN_TCP_SYN_PORT		42424

// E_FTSCAN_ECHO_PING scan states (for the scan result's reason field)
enum e_ftscan_echo_ping_state {
	E_FTSCAN_ECHO_PING_TIMEOUT,
	E_FTSCAN_ECHO_PING_TTL,
	E_FTSCAN_ECHO_PING_DEST_UNREACH,
	E_FTSCAN_ECHO_PING_ECHO_REPLY,
};

// E_FTSCAN_ECHO_PING scan states (for the scan result's reason field)
enum e_ftscan_tcp_syn_state {
	E_FTSCAN_TCP_SYN_TIMEOUT,
	E_FTSCAN_TCP_SYN_ICMP_DEST_UNREACH,
	E_FTSCAN_TCP_SYN_TCP_RST,
	E_FTSCAN_TCP_SYN_TCP_SYN,
};

/*
** t_scanres: Result of a scan
**
** rtt: round trip time (from sending to receive event)
** ttl: ip time to live value
** open: is the port open (syn) or the host up (ping)
** reason: scan specific value indicating the reason for the open status
** sequence: echo reply sequence value or simple response counter (starts at 1)
*/
typedef struct		s_scan_result
{
	struct timeval	rtt;
	uint8_t			ttl;
	int				open;
	int				reason;
	uint16_t		sequence;
}					t_scanres;

/*
** t_scan_control: Structure representing an individual scan.
**
** ip: destination IP address to scan
** port: destination port if is a port scan
** sendfd: socket to send scan probes
** recvfd: socket to receive the replies
** result: result for the current scan (if sent_ts is set)
** sent_ts: timestamp of last sent probe
** timeout: return a timeout event after this long (0 for no timeout)
** sequence: numbers of probes sent
** payload_size: size of the probe payload if any
** payload: payload data
*/
typedef struct			s_scan_control
{
	t_ip				ip;
	uint16_t			port;
	t_send_socket		sendfd;
	t_recv_socket		recvfd;
	t_scanres			result;
	struct timeval		sent_ts;
	struct timeval		timeout;
	int					sequence;
	size_t				payload_size;
	uint8_t				payload[MAX_PACKET_PAYLOAD_SIZE];
}						t_scan_control;

/*
** t_scan: Int value representing an individual scan. Contains the id of the
** scan on the lowest 10 bits (which means max number of scans is 2^10-1) and
** the type of scan on the next 6 bits. The last bits are left unused. The
** t_scan value can be negative if the scan open call fails.
*/
typedef int32_t		t_scan;

/*
** Fields of the t_scan type: The t_scan field takes 16 bits in total. It could
** take more but this way we can use the t_scan value directly for the packet id
** of scan probes. This is way easier.
**
** The lowest 10 bits represent the scan id (which is the index in the global
** scan_list array), and the upper 6 encode the type of the scan.
*/
# define SCAN_HANDLE_BITLEN		16
# define SCAN_ID_BITLEN			10
# define SCAN_TYPE_BITLEN		(SCAN_HANDLE_BITLEN - SCAN_ID_BITLEN)

// Maximum number of scans by type
# define MAX_SCAN_COUNT			((1 << SCAN_ID_BITLEN) - 1)	// 2^10-1=1023
# define MAX_TYPE_COUNT			((1 << SCAN_TYPE_BITLEN) - 1)	// 2^6-1=63

// t_scan field getters
# define GET_SCAN_ID(scan)		((uint16_t)(scan & MAX_SCAN_COUNT))
# define GET_SCAN_TYPE(scan)	((enum e_ftscan_type)(scan >> SCAN_ID_BITLEN))

// Maximum total number of scans
# define MAX_GLOBAL_SCAN_COUNT	(MAX_SCAN_COUNT * FTSCAN_TYPE_COUNT)

// List of every current scan (only used internally, never exposed to the user)
# ifdef THREAD_SAFE
	extern __thread t_scan_control	**g_scan_list[FTSCAN_TYPE_COUNT];
	extern __thread uint16_t		g_scan_count[FTSCAN_TYPE_COUNT];
# else
	extern t_scan_control			**g_scan_list[FTSCAN_TYPE_COUNT];
	extern uint16_t					g_scan_count[FTSCAN_TYPE_COUNT];
# endif

enum e_pollsc_events
{
	E_POLLSC_NONE		= 0x0000,
	E_POLLSC_REPLY		= 0x0001,
	E_POLLSC_TIMEOUT	= 0x0002,
	E_POLLSC_ERROR		= 0x0004,
};

/*
** t_pollsc: Works like poll's pollfd structure but for scans and every events
** are listened for.
*/
typedef struct		s_poll_scan
{
	t_scan			scan;
	uint16_t		events;
}					t_pollsc;

/*
** Scan functions
**
** Every scan function will return -1 on error and set ft_errno on error, except
** close functions which cannot fail. They will return 0 on success, except for
** ft_scan_poll which returns the number of scans on which we have an event.
*/

// Initialize scan data
t_scan	ft_echo_ping_open(t_ip *ip, struct timeval *timeout);
t_scan	ft_tcp_syn_open(t_ip *ip, uint16_t port, struct timeval *timeout);
int		ft_scan_set_timeout(t_scan scan, struct timeval *timeout);
int		ft_scan_set_payload(t_scan scan, uint8_t *payload, size_t payload_len);

// Synchrone scan
int		ft_echo_ping(t_scanres *result, t_scan scan);
int		ft_tcp_syn(t_scanres *result, t_scan scan);

// Asynchrone scan
int		ft_scan_send(t_scan scan);
int		ft_scan_poll(t_pollsc *scans, int count, struct timeval *t);
int		ft_scan_result(t_scanres *result, t_scan scan);

// Scan cleanup
void	ft_scan_close(t_scan scan);
void	ft_scan_close_all(void);

// Internal functions (should not be used outside the libft!!!)
t_scan			ft_add_new_scan(enum e_ftscan_type type, t_ip *ip,
	uint16_t port);
t_scan_control	*ft_get_scan(t_scan scan);
uint8_t			ft_get_scan_protocol(enum e_ftscan_type type, int domain);
int				ft_scan_set_filter(t_scan scan);
int				ft_packet_filter_tcp_syn(t_recv_socket recvfd,
	t_filter_spec *spec);
int				ft_packet_filter_echo_ping(t_recv_socket recvfd,
	t_filter_spec *spec);
int				ft_scan_recv(t_scan scan, int wait, struct timeval *recv_ts);
int				ft_echo_ping_parse_reply(t_scan_control *scan_ctrl,
	t_packet *reply, struct timeval *recv_ts);
int				ft_tcp_syn_parse_reply(t_scan_control *scan_ctrl,
	t_packet *reply, struct timeval *recv_ts);

#endif
