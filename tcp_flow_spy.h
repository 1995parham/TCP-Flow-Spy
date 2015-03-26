/*
 * In The Name Of God
 * ========================================
 * [] File Name : tcp_flow_spy.h
 *
 * [] Creation Date : 27-03-2015
 *
 * [] Last Modified : Fri 27 Mar 2015 02:12:08 AM IRDT
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#ifndef TCP_FLOW_SPY_H
#define TCP_FLOW_SPY_H

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19))
#define SPY_COMPAT 18
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,34))
#define SPY_COMPAT 32
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
#define SPY_COMPAT 34
#else
#define SPY_COMPAT 35
#endif

#define HASHTABLE_SIZE 1357
#define MAX_CONTINOUS 128
#define SECTION_COUNT (bufsize / MAX_CONTINOUS)

#define NUMBER_OF_BUCKETS   10

struct tcp_flow_log {
	struct timespec first_packet_tstamp;
	struct timespec last_packet_tstamp;
	struct timespec last_printed_tstamp;
	__be32 saddr, daddr;
	__be16 sport, dport;
	/* No of received packets */
	u32 recv_count;
	/* No of sent packets */
	u32 snd_count;
	/* Avg length of the packet */
	u64 recv_size;
	/* Total size of packets in byte */
	u64 snd_size;
	u32 last_recv_seq;
	u32 last_snd_seq;
	u32 out_of_order_packets;
	u32 total_retransmissions;
	u32 snd_cwnd_clamp;
	u32 ssthresh;
	u32 srtt;
	u32 rttvar;
	u32 last_cwnd;
	u32 rto;
	int used;
	u32 snd_cwnd_histogram[NUMBER_OF_BUCKETS];
	spinlock_t lock;
	u32 buff_size;
	u32 max_buff_size;
	struct tcp_flow_log *used_thread_next;
	struct tcp_flow_log *used_thread_prev;
	struct tcp_flow_log *next;
	struct tcp_flow_log *prev;
};

static struct {
	spinlock_t lock;
	wait_queue_head_t wait;
	struct timespec	start;
	struct timespec last_update;
	struct timespec last_read;
	struct tcp_flow_log *available;
	struct tcp_flow_log **storage;
	struct tcp_flow_log *finished;
	struct tcp_flow_log *used;
} tcp_flow_spy;

struct hashtable_entry {
	spinlock_t lock;
	struct tcp_flow_log *head;
	struct tcp_flow_log *tail;
};

static struct {
	struct hashtable_entry *entries;
} tcp_flow_hashtable;

#endif
