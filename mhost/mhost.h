#ifdef WIN32
#pragma comment(lib, "Dnsapi.lib")

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

typedef LONG    DNS_STATUS, *PDNS_STATUS;

typedef enum{
    //  In Win2K
    DnsConfigPrimaryDomainName_W,
    DnsConfigPrimaryDomainName_A,
    DnsConfigPrimaryDomainName_UTF8,

    //  Not available yet
    DnsConfigAdapterDomainName_W,
    DnsConfigAdapterDomainName_A,
    DnsConfigAdapterDomainName_UTF8,

    //  In Win2K
    DnsConfigDnsServerList,

    //  Not available yet
    DnsConfigSearchList,
    DnsConfigAdapterInfo,

    //  In Win2K
    DnsConfigPrimaryHostNameRegistrationEnabled,
    DnsConfigAdapterHostNameRegistrationEnabled,
    DnsConfigAddressRegistrationMaxCount,

    //  In WindowsXP
    DnsConfigHostName_W,
    DnsConfigHostName_A,
    DnsConfigHostName_UTF8,
    DnsConfigFullHostName_W,
    DnsConfigFullHostName_A,
    DnsConfigFullHostName_UTF8

    //  In XP-SP1 (Server.net)
}
DNS_CONFIG_TYPE;

DNS_STATUS WINAPI DnsQueryConfig(
    IN      DNS_CONFIG_TYPE     Config,
    IN      DWORD               Flag,
    IN      PWSTR               pwsAdapterName,
    IN      PVOID               pReserved,
    OUT     PVOID               pBuffer,
    IN OUT  PDWORD              pBufferLength
    );

#ifdef __cplusplus
}
#endif  // __cplusplus
#endif

unsigned int GetDNSServs(unsigned int *arr, unsigned int arrs){
#ifdef WIN32
	DWORD len = arrs*4;
	DNS_STATUS res = DnsQueryConfig(DnsConfigDnsServerList, 0, 0, NULL, arr, &len);
	if(!res && len>4){
		res=arr[0]; memcpy(&arr[0], &arr[1], len-4);
		for(int i=0; i<res; i++){ arr[i]=htonl(arr[i]); }
		return res;
	}
	return 0;
#else
	MString file=LoadFile("/etc/resolv.conf"); int res=0;
	ExplodeLine el; int els=explode(el, file, file, (unsigned char*)"\n", 1);
	for(int i=0; i<els; i++){
		VString e=el.el(i); if(e && e.endo()==13) e.sz--;
		VString t=PartLine(e, e, " "); dspacev(e, 7);
		if(t!="nameserver") continue;
		if(res<arrs){
			arr[res]=ipstoi(e);
			if(arr[res]) res++;
		}
	}
	return res;
#endif

return 0;
}