/* sys */
#include "config.h"
#include "private.h"

/* app */
#include "env.h"
#include "log.h"

// clang-format off
const char *emu_env = 
 "{"
 "\"active\":\"a\","
 "\"boot_img\":\"bootz 0x22000000 - 0x21000000;\","
 "\"bootcmd\":\"run load_env; run load_arg; run read_dtb; run read_img; run boot_img;\","
 "\"bootcmd_nfs\":\"run load_arg_nfs; run load_dtb; run load_img; run read_dtb; run read_img; run boot_img;\","
 "\"console\":\"console=ttyS0,115200 earlyprintk\","
 "\"ethaddr\":\"54:10:ec:f1:a1:05\","
 "\"fdtcontroladdr\":\"27962318\","
 "\"foo\":\"tom\","
 "\"load_arg\":\"setenv bootargs '$console $mtdparts rootfstype=ubifs $ubi root=ubi0:rootfs'\","
 "\"load_arg_nfs\":\"setenv bootargs '$console root=/dev/nfs ip=$target_ip:$nfs_ip:$nfs_ip:255.255.255.0:atx-demo::off nfsroot=$nfs_ip:$nfs_path,$nfs_opts'\","
 "\"load_dtb\":\"if test '${active}' = 'a'; then setenv dtb 0x00000000; else setenv dtb 0x0fc00000;fi;\","
 "\"load_env\":\"run load_dtb; run load_img; run load_ubi;\","
 "\"load_img\":\"if test '${active}' = 'a'; then setenv img 0x00080000; else setenv img 0xdfc80000;fi;\","
 "\"load_ubi\":\"if test '${active}' = 'a'; then setenv ubi 'ubi.mtd=rootfs_a'; else setenv ubi 'ubi.mtd=rootfs_b'; fi;\","
 "\"mtdparts\":\"mtdparts=atmel_nand:512k(dtb_a),6M(kernel_a)ro,257425408(rootfs_a),512k(dtb_b),6M(kernel_b)ro,257425408(rootfs_b)-(data)\","
 "\"nfs_ip\":\"192.168.0.4\","
 "\"nfs_opts\":\"tcp,nolock,wsize=4096,rsize=4096,nfsvers=3 rw\","
 "\"nfs_path\":\"/home/thomas/Git/altronix/gateway/buildroot-at91/output/images/rootfs\","
 "\"read_dtb\":\"nand read 0x21000000 $dtb 0x80000;\","
 "\"read_img\":\"nand read 0x22000000 $img 0x600000;\","
 "\"target_ip\":\"192.168.0.66\""
 "}";
// clang-format on

int
env_init(env_s* env, const char* path)
{
    memset(env, 0, sizeof(env_s));
    return 0;
}

void
env_deinit(env_s* env)
{
    memset(env, 0, sizeof(env_s));
}

int
env_read(env_s* env, char* buff, uint32_t* b)
{
    uint32_t sz = *b;
    *b = snprintf(buff, sz, "%s", emu_env);
    return sz < *b ? 0 : -1;
}

const char*
env_read_val(env_s* env, const char* key)
{
    // TODO parse our emu and return the string
    return NULL;
}

int
env_write(env_s* env)
{
    // TODO
    return -1;
}
