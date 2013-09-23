#ifndef ETHERNET_CONFIG_H
#define ETHERNET_CONFIG_H

#ifndef CONFIG_DISABLE_DNS
#define HAS_DNS
#endif /* CONFIG_DISABLE_DNS */

#ifndef CONFIG_DISABLE_DHCP
#define HAS_DHCP
#endif /* CONFIG_DISABLE_DHCP */

#ifndef CONFIG_DISABLE_UNNECESSARY_ETHERNET_BEGIN
#define HAS_UNNECESSARY_ETHERNET_BEGIN
#endif /* CONFIG_DISABLE_UNNECESSARY_ETHERNET_BEGIN */

#ifndef CONFIG_DISABLE_ETHERNET_CLIENT
#define HAS_ETHERNET_CLIENT
#endif /* CONFIG_DISABLE_ETHERNET_CLIENT */

#ifndef CONFIG_DISABLE_ETHERNET_SERVER
#define HAS_ETHERNET_SERVER
#endif /* CONFIG_DISABLE_ETHERNET_SERVER */

#ifndef CONFIG_DISABLE_STREAM_PRINT
#define HAS_STREAM_PRINT
#endif /* CONFIG_DISABLE_STREAM_PRINT */

#ifndef CONFIG_DISABLE_ETHERNET_PRINT
#define HAS_ETHERNET_PRINT
#endif /* CONFIG_DISABLE_ETHERNET_PRINT */

#endif /* ETHERNET_CONFIG_H */

