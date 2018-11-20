#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */
#include <linux/netfilter.h> 
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/compiler.h>
//#include <linux/smp_lock.h>
#include <net/tcp.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>


#define IP_LENGTH 16
#define NUMBER_OF_NODES 50
#define MAX_FILE_LENGTH IP_LENGTH*NUMBER_OF_NODES
#define OUTPUT_FILE "/tmp/test"

MODULE_AUTHOR ("Wenbing Tang");
MODULE_DESCRIPTION ("Connection Tracking") ;
MODULE_LICENSE("GPL");

struct nf_hook_ops *reg;

struct data_node {
	char source[IP_LENGTH];
	char des[IP_LENGTH];
	unsigned int count;
};

struct connection_data {
	struct data_node data_node[NUMBER_OF_NODES];
	unsigned int size;
};

struct connection_data data;

// implement write file function
static void write_file(char *filename, char *data)
{
  struct file *file;
  loff_t pos = 0;

  mm_segment_t old_fs = get_fs();
  set_fs(KERNEL_DS);

  file = filp_open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644);

  if (file) {
    vfs_write(file, data, strlen(data), &pos);
    fput(file);
  }
  filp_close(file, NULL);
  set_fs(old_fs);
}

// update the output file
void update_file(char source[], char des[])
{
	char file_data[MAX_FILE_LENGTH];
	int i;
	char temp[50];
	
	for (i=0; i<data.size; i++)
	{
		//Find the data from source to des
		if(!strcmp(data.data_node[i].source, source) && !strcmp(data.data_node[i].des, des))
		{
			data.data_node[i].count++;
			break;
		}
	}
	
	if (i == data.size) //Can't find the data from source to des
	{
		sprintf(data.data_node[i].source, "%s", source);
		sprintf(data.data_node[i].des, "%s", des);
		data.data_node[i].count = 1;
		data.size++;
	}
	else if (i+1 > NUMBER_OF_NODES)
	{
		printk (KERN_INFO "Over the limit of NUMBER_OF_NODES\n");
		return;
	}
	
	//Write the data to file
	snprintf(file_data, sizeof(file_data), "%s\t%s\t%s\n", "From_IP_Address", "To_IP_Address", "Package_Count");
	for (i=0; i<data.size; i++)
	{
		
		snprintf(temp, sizeof(temp), "%s\t%s\t%d\n", data.data_node[i].source, data.data_node[i].des, data.data_node[i].count);
		strcat(file_data, temp);
	}	
	
	printk (KERN_INFO "connection_tracking: file_data = %s\n", file_data);
	write_file(OUTPUT_FILE, file_data);
}


unsigned int connection_tracking_hook (unsigned int hooknum,
				    struct sk_buff *skb,
				    const struct net_device *in,
				    const struct net_device *out,
				    int (*okfn)(struct sk_buff *)) 
{
	struct tcphdr *tcp;
  struct tcphdr _tcph;
	struct iphdr *ip;
	char source[IP_LENGTH];
	char des[IP_LENGTH];
	
	/* get the tcp-header for the packet */
	tcp = skb_header_pointer(skb, ip_hdrlen(skb), sizeof(struct tcphdr), &_tcph);
	if (!tcp) {
		printk (KERN_INFO "Could not get tcp-header!\n");
		return NF_ACCEPT;
	}
		

	ip = (struct iphdr *)skb_network_header(skb);
	if(ip)
	{
		snprintf(source, 16, "%pI4", &ip->saddr);
		snprintf(des, 16, "%pI4", &ip->daddr);
		
		//192.168.56.1 is the host DHCP server.
		if (!strcmp(source, "192.168.56.1") || !strcmp(des, "192.168.56.1"))
		{
			return NF_ACCEPT;	
		}
		
		printk (KERN_INFO "connection_tracking: Source address IP4 = %s\n", source);
		printk (KERN_INFO "connection_tracking: Destination address IP4 = %s\n", des);
		
		update_file(source, des);		
	}
	
  return NF_ACCEPT;	
}

EXPORT_SYMBOL(connection_tracking_hook);

int init_module(void)
{
  int errno;

  // initialization
	data.size = 0;

  /* allocate space for hook */
  reg = kmalloc (sizeof (struct nf_hook_ops), GFP_KERNEL);
  if (!reg) 
  {
    return -ENOMEM;
  }

  /* fill it with the right data */
  reg->hook = connection_tracking_hook; /* the procedure to be called */
  reg->pf = PF_INET;
  reg->hooknum = NF_INET_LOCAL_IN | NF_INET_LOCAL_OUT; 
  //reg->hooknum = NF_IP_LOCAL_IN | NF_IP_LOCAL_OUT; 
  
  errno = nf_register_hook (reg); /* register the hook */
  if (errno) 
  {
    printk (KERN_INFO "Connection tracking extension could not be registered!\n");
    kfree (reg);
  } 
  else 
  {
    printk(KERN_INFO "Connection tracking extensions module loaded\n");
  }

  // A non 0 return means init_module failed; module can't be loaded.
  return errno;
}


void cleanup_module(void)
{
	nf_unregister_hook (reg); /* restore everything to normal */
	kfree (reg);
	printk(KERN_INFO "Connection tracking extensions module unloaded\n");
}   