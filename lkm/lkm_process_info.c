#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/time.h> 
#include <linux/init.h>  
#include <linux/netfilter.h>
#include <linux/netdevice.h>  
#include <linux/netfilter_ipv4.h>
#include <linux/net.h>
#include <net/ip.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#define NETLINK_USER 31

#define pc_num 50
typedef struct package_count
{
    int saddr;
    int daddr;
    int count;
}package_count;

static package_count pc[pc_num];
static int length_pc;
static char msg[1024];

unsigned int my_hookfn(void * priv, struct sk_buff *skb, const struct nf_hook_state *state)
{	
	bool match;
	int i;
	struct iphdr *iph;
	iph = ip_hdr(skb);
	
	match = false;	
	for(i=0; i<length_pc; i++)
	{
		if(pc[i].saddr == iph->saddr&&pc[i].daddr == iph->daddr)
		{
			pc[i].count = pc[i].count+1;
			match = true;
			printk(KERN_INFO"src IP %d \n", iph->saddr);
			printk(KERN_INFO"length pc increased %d \n", length_pc);
			break;
		}
	}
	if(!match)
	{
		if(!(length_pc<pc_num)){
			printk(KERN_INFO"overfill %d \n", iph->saddr);
			length_pc = pc_num-1;
		}
		else{
			pc[length_pc].saddr = iph->saddr;
			pc[length_pc].daddr = iph->daddr;
			pc[length_pc].count = pc[length_pc].count+1;
			length_pc = length_pc+1;
			printk(KERN_INFO"length pc increased %d \n", length_pc);
		}
		
		
	}
	return NF_ACCEPT;
}

static struct nf_hook_ops nfho =
{
    .hook = my_hookfn,
    .pf = PF_INET,
    .hooknum = NF_INET_PRE_ROUTING,
    .priority = NF_IP_PRI_FIRST,
};

//print file part
struct sock *nl_sk = NULL;

static void con_msg(void){
	int i;
	snprintf(msg,sizeof(msg),"");
	strcat(msg,"{");
	for(i=0;i<length_pc;i++){
		char str[32];
		snprintf(str,sizeof(str),"\"%d\": {",i);
		strcat(msg,str);
		strcat(msg,"\"from\": ");
		snprintf(str,sizeof(str),"\"%pI4\" ,",&pc[i].saddr);
		strcat(msg,str);
		strcat(msg,"\"to\": ");
		snprintf(str,sizeof(str),"\"%pI4\" ,",&pc[i].daddr);
		strcat(msg,str);
		snprintf(str,sizeof(str),"\"pakets\": %d ",pc[i].count);
		strcat(msg,str);
		if(i == length_pc -1){
			strcat(msg,"}\n");
		}
		else{
			strcat(msg,"},\n");
		}
	}
	strcat(msg,"}");	
}

static void hello_nl_recv_msg(struct sk_buff *skb) {
	struct nlmsghdr *nlh;
	int pid;
	struct sk_buff *skb_out;
	int msg_size;
	int res;
	con_msg();
	printk(KERN_INFO "Entering: %s\n", __FUNCTION__);
	msg_size=strlen(msg);
	nlh=(struct nlmsghdr*)skb->data;
	printk(KERN_INFO "Netlink received msg payload:%s\n",(char*)nlmsg_data(nlh));
	pid = nlh->nlmsg_pid; /*pid of sending process*/
	skb_out = nlmsg_new(msg_size,0);

	if(!skb_out){
    		printk(KERN_ERR "Failed to allocate new skb\n");
    		return;
	} 
	nlh=nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);  
	NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */

	strncpy(nlmsg_data(nlh),msg,msg_size);
	res=nlmsg_unicast(nl_sk,skb_out,pid);

	if(res<0)
		printk(KERN_INFO "Error while sending bak to user\n");
}

static int __init hello_init(void) {
	printk("Entering: %s\n",__FUNCTION__);
//This is for 3.6 kernels and above.
	struct netlink_kernel_cfg cfg = {
		.input = hello_nl_recv_msg,
	};
	nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);

	if(!nl_sk){
		printk(KERN_ALERT "Error creating socket.\n");
		return -10;
	}

	
	//nethook part

	int i;
	if (nf_register_net_hook(&init_net, &nfho))
	{
		return -1;
	}
	for(i=0;i<pc_num;i++){
		pc[i].count = 0;
	}
	length_pc = 0;
	return 0;
}

static void __exit hello_exit(void) {
	printk(KERN_INFO "exiting hello module\n");
	netlink_kernel_release(nl_sk);
	nf_unregister_net_hook(&init_net, &nfho);
}

module_init(hello_init); 
module_exit(hello_exit);

MODULE_LICENSE("GPL");
