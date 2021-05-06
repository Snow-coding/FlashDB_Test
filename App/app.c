#include "stdio.h"
#include "../Fal/inc/fal.h"
#include "../Fal/inc/fal_def.h"
#include "../Fal/inc/fal_cfg.h"

#include "../FlashDB/inc/flashdb.h"

#define FDB_LOG_TAG "[main]"

static uint32_t boot_count = 0;
static time_t boot_time[10] = {0, 1, 2, 3};
/* default KV nodes */
static struct fdb_default_kv_node default_kv_table[] = {
        {"username", "armink", 0}, /* string KV */
        {"password", "123456", 0}, /* string KV */
        {"boot_count", &boot_count, sizeof(boot_count)}, /* int type KV */
 //       {"boot_time", &boot_time, sizeof(boot_time)},    /* int array type KV */
};
/* KVDB object */
static struct fdb_kvdb kvdb = { 0 };
/* TSDB object */
struct fdb_tsdb tsdb = { 0 };
/* counts for simulated timestamp */
static int counts = 0;

extern void kvdb_basic_sample(fdb_kvdb_t kvdb);
extern void kvdb_type_string_sample(fdb_kvdb_t kvdb);
extern void kvdb_type_blob_sample(fdb_kvdb_t kvdb);
extern void tsdb_sample(fdb_tsdb_t tsdb);

static void lock(fdb_db_t db)
{
    //__disable_irq();
}

static void unlock(fdb_db_t db)
{
    //__enable_irq();
}

static fdb_time_t get_time(void)
{
    /* Using the counts instead of timestamp.
     * Please change this function to return RTC time.
     */
    return ++counts;
}

#define FDB_LOG_TAG "[sample][kvdb][basic]"

void kvdb_basic_sample(fdb_kvdb_t kvdb)
{
    struct fdb_blob blob;
    int boot_count = 0;

    FDB_INFO("==================== kvdb_basic_sample ====================\n");

    { /* GET the KV value */
        /* get the "boot_count" KV value */
        fdb_kv_get_blob(kvdb, "boot_count", fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the 'boot_count' value is %d\n", boot_count);
        } else {
            FDB_INFO("get the 'boot_count' failed\n");
        }
    }

    { /* CHANGE the KV value */
        /* increase the boot count */
        boot_count ++;
        /* change the "boot_count" KV's value */
        fdb_kv_set_blob(kvdb, "boot_count", fdb_blob_make(&blob, &boot_count, sizeof(boot_count)));
        FDB_INFO("set the 'boot_count' value to %d\n", boot_count);
    }

    FDB_INFO("===========================================================\n");
}

static int fdb_init(void)
{
    fdb_err_t result;

    /* KVDB Sample */
    struct fdb_default_kv default_kv;

    default_kv.kvs = default_kv_table;
    default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
    /* set the lock and unlock function if you want */
    fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, lock);
    fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, unlock);
    /* Key-Value database initialization
        *
        *       &kvdb: database object
        *       "env": database name
        * "fdb_kvdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
        *              Please change to YOUR partition name.
        * &default_kv: The default KV nodes. It will auto add to KVDB when first initialize successfully.
        *        NULL: The user data if you need, now is empty.
        */
    result = fdb_kvdb_init(&kvdb, "env", "test1", &default_kv, NULL);

    if (result != FDB_NO_ERR) {
        return -1;
    }

    /* run basic KV samples */
    kvdb_basic_sample(&kvdb);
    //kvdb_basic_sample(&kvdb);
    /* run string KV samples */
    //kvdb_type_string_sample(&kvdb);
    /* run blob KV samples */
    //kvdb_type_blob_sample(&kvdb);
    return 0;
}

static void flash_read(char *name,uint8_t addr,uint8_t size)
{
    struct fal_partition *partition = fal_partition_find(name);
    uint8_t data[128];
    fal_partition_read(partition,addr,data,size);
}

static void flash_write(char *name,uint8_t addr,uint8_t *data,uint8_t size)
{
    struct fal_partition *partition = fal_partition_find(name);

    fal_partition_write(partition,addr,data,size);
}

static void flash_erase(char *name,uint8_t addr,uint16_t size)
{
    struct fal_partition *partition = fal_partition_find(name);

    fal_partition_erase(partition,addr,size);
}

static void fal_test(void)
{
    printf("main\r\n");
    fal_init();
    fal_show_part_table();
    flash_erase("test1",0,1024);
    //flash_erase("test2",0,2048);
    return;
    flash_read("test1",0,32);
    uint8_t data[64] = {12,3,4,5,6,7,8,9,10};
    //flash_write("test1",0,data,4);
    //flash_read("test1",0,4);

    flash_erase("test1",0,10);
    flash_write("test1",0,data+4,10);
    flash_read("test1",0,4);
}

static void fdb_test(void)
{
    fdb_init();

    flash_cache_file_store();
}

int main(void)
{
    //fal_test();
    fdb_test();
    return 0;
}