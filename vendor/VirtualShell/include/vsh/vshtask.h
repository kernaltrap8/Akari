#ifndef __VSHTASK_H__
#define __VSHTASK_H__


extern "C" int32_t vshtask_0F80B71F(const char *path);  // auth_module, checks sprx SCE header	int vshtask_F80B71F(char * path)

extern "C" int32_t vshtask_166551C5(char *app_id);      // RTC Alarm Unregister

//extern "C" int32_t vshtask_668E3C94(char * app_id?, uint8_t [0x4C]);  // some RTC Alarm Register

extern "C" int32_t vshtask_8D03E0FD(void);              // ?

extern "C" int32_t vshtask_784023D0(uint8_t status[0x1D0]); //	RTC Alarm Get Status	int vshtask_784023D0


extern "C" int32_t vshtask_A02D46E7(int32_t arg, const char *msg);  // vshtask_notification_msg()  -> Displays a notification in XMB, calls vshcommon_A20E43DB with texture "tex_notification_info"
#define vshtask_notify(msg) vshtask_A02D46E7(0, msg)

#endif // __VSHTASK_H__
