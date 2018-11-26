#include <linux/netfilter.h>
#include <linux/init.h>
#include <linux/module.h>

static unsigned int my_hookfn(void * priv, struct sk_buff *skb, const struct nf_hook_state *state);

static struct package_count
{
    __be32 saddr;
    int count = 0;
}
static package_count pc[100];
static int length_pc = 0;
unsigned int my_hookfn(void * priv, struct sk_buff *skb, const struct nf_hook_state *state)
{

    struct iphdr *iph;
    iph = ip_hdr(skb);
    printk(KERN_INFO"src IP %pI4\n", &iph->saddr);
printk(KERN_INFO"length pc increased %p \n", length_pc);
    bool match = false;
    for(int i=0; i<length_pc; i++)
    {
        if(pc[i].saddr == &iph->saddr)
        {
            pc[i].count++;
            match = true;
            break;
        }
    }
    if(!match)
    {
        pc[length_pc].saddr = &iph->saddr;
        pc[length_pc].count++;
        length_pc++;
printk(KERN_INFO"length pc increased %p c\n", length_pc);
        if(!length_pc<100){
 printk(KERN_INFO"overfill %pI4\n", &iph->saddr);
length_pc = 99;
        }
    }

    return NF_ACCEPT;
}

static struct nf_hook_ops nfho =
{
    .hook = my_hookfn,
    .pf = PF_INET,
    .hooknum = NF_IP_LOCAL_IN,
    .priority = NF_IP_PRI_FIRST,
};


static int __init sknf_init(void)
{
    if (nf_register_net_hook(&init_net, &nfho))
    {
        return -1;
    }
    return 0;
}


static void __exit sknf_exit(void)
{
printk(KERN_INFO"IP %p  \n", length_pc);
for(int i=0;i<length_pc;i++){
printk(KERN_INFO"IP %p ; count %p \n", pc[i].saddr,pc[i].count);
}
    nf_unregister_net_hook(&init_net, &nfho);
}
module_init(sknf_init);
module_exit(sknf_exit);
MODULE_AUTHOR("Liqun");
MODULE_LICENSE("GPL");
