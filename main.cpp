#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/videodev2.h>
#include "uAPI2/rk_aiq_user_api2_sysctl.h"
#include "common/mediactl/mediactl.h"

#define DBG(...) do { if(!silent) printf("DBG: " __VA_ARGS__);} while(0)
#define ERR(...) do { fprintf(stderr, "ERR: " __VA_ARGS__); } while (0)

#define RKAIQ_FILE_PATH_LEN 64
#define IQ_PATH "/etc/iqfiles/"

struct rkaiq_media_info {
    char sd_isp_path[RKAIQ_FILE_PATH_LEN];
    char vd_params_path[RKAIQ_FILE_PATH_LEN];
    char vd_stats_path[RKAIQ_FILE_PATH_LEN];
    char mainpath[RKAIQ_FILE_PATH_LEN];
    char sensor_entity_name[32];

    char mdev_path[32];
    int available;
    rk_aiq_sys_ctx_t* aiq_ctx;
};

static struct rkaiq_media_info media_info;
static int silent = 0;


static int rkaiq_get_devname(struct media_device *device, const char *name, char *dev_name)
{
    const char *devname;
    struct media_entity *entity =  NULL;

    entity = media_get_entity_by_name(device, name, strlen(name));
    if (!entity)
        return -1;

    devname = media_entity_get_devname(entity);

    if (!devname) {
        fprintf(stderr, "can't find %s device path!", name);
        return -1;
    }

    strncpy(dev_name, devname, RKAIQ_FILE_PATH_LEN);

    DBG("get %s devname: %s\n", name, dev_name);

    return 0;
}

int rkaiq_get_media_info(struct rkaiq_media_info *media_info)
{
    struct media_device *device = NULL;
    const char *sensor_name;
    int ret;

    device = media_device_new (media_info->mdev_path);
    if (!device)
        return -ENOMEM;
    /* Enumerate entities, pads and links. */
    ret = media_device_enumerate (device);
    if (ret)
        return ret;
    if (!ret) {
        /* Try rkisp */
        ret = rkaiq_get_devname(device, "rkisp-isp-subdev",
                                media_info->sd_isp_path);
        ret |= rkaiq_get_devname(device, "rkisp-input-params",
                                media_info->vd_params_path);
        ret |= rkaiq_get_devname(device, "rkisp-statistics",
                                media_info->vd_stats_path);
        ret |= rkaiq_get_devname(device, "rkisp_mainpath",
                                media_info->mainpath);
    }
    if (ret) {
        fprintf(stderr, "Cound not find rkisp dev names, skipped %s\n", media_info->mdev_path);
        media_device_unref (device);
        return ret;
    }

    sensor_name = rk_aiq_uapi2_sysctl_getBindedSnsEntNmByVd(media_info->mainpath);
    if (sensor_name == NULL || strlen(sensor_name) == 0) {
        fprintf(stderr, "ERR: No sensor attached to %s\n", media_info->mdev_path);
        media_device_unref (device);
        return -EINVAL;
    }

    strcpy(media_info->sensor_entity_name, sensor_name);

    media_device_unref (device);

    return ret;
}


int main()
{
    const uint32_t width = 1280;
    const uint32_t height = 960;
    const rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
    rk_aiq_sys_ctx_t* aiq_ctx = nullptr;

    sprintf(media_info.mdev_path, "/dev/media0");
    if (rkaiq_get_media_info(&media_info)) {
        ERR("Bad media topology for: %s\n", media_info.mdev_path);
        return 1;
    }
    fprintf(stdout, "sensor_entity_name: %s\n", media_info.sensor_entity_name);

    // rk_aiq_static_info_t aiq_static_info;
    // rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId( 0, &aiq_static_info );

    // aiq_ctx = rk_aiq_uapi2_sysctl_init( aiq_static_info.sensor_info.sensor_name,
    //                                     "/usr/share/rockchip-iqfiles-rk356x",
    //                                     nullptr,
    //                                     nullptr );
    // rk_aiq_uapi2_sysctl_prepare( aiq_ctx, width, height, hdr_mode );
    // rk_aiq_uapi2_sysctl_start( aiq_ctx );
    // std::cout << "sleep 5" << std::endl;
    // sleep(5);
    // rk_aiq_uapi2_sysctl_stop( aiq_ctx );
    // rk_aiq_uapi2_sysctl_deinit( aiq_ctx );
    return 0;
}

