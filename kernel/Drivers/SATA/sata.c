#include "sata.h"
#define	SATA_SIG_ATA	0x00000101	// SATA drive
#define	SATA_SIG_ATAPI	0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB	0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM	0x96690101	// Port multiplier

#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4

#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3

void probe_port(HBA_MEM *abar)
{
	// Search disk in impelemented ports
	uint32_t pi = abar->pi;
	int i = 0;
	while (i<32)
	{
		if (pi & 1)
		{
			int dt = check_type(&abar->ports[i]);
			if (dt == AHCI_DEV_SATA)
			{
				trace_ahci("SATA drive found at port %d\n", i);
			}
			else if (dt == AHCI_DEV_SATAPI)
			{
				trace_ahci("SATAPI drive found at port %d\n", i);
			}
			else if (dt == AHCI_DEV_SEMB)
			{
				trace_ahci("SEMB drive found at port %d\n", i);
			}
			else if (dt == AHCI_DEV_PM)
			{
				trace_ahci("PM drive found at port %d\n", i);
			}
			else
			{
				trace_ahci("No drive found at port %d\n", i);
			}
		}

		pi >>= 1;
		i ++;
	}
}

// Check device type
static int check_type(HBA_PORT *port)
{
	uint32_tssts = port->ssts;

	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;

	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;

	switch (port->sig)
	{
	case SATA_SIG_ATAPI:
		return AHCI_DEV_SATAPI;
	case SATA_SIG_SEMB:
		return AHCI_DEV_SEMB;
	case SATA_SIG_PM:
		return AHCI_DEV_PM;
	default:
		return AHCI_DEV_SATA;
	}
}
