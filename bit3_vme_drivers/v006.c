// ****************************************************************************
// *     Filename:  v006.c
// *  Description:  
// *   Created by:  Nam Tran (nam at kuno-g.phys.sci.osaka-u.ac.jp)
// *     Time tag:  2013-05-15 14:51:12
// ****************************************************************************
#include "v006.h"
#include "bt617.h"

int v006_clear(MVME_INTERFACE *vme, mvme_addr_t base)
{
	mvme_set_am(vme, V006_AM);
	mvme_set_dmode(vme, V006_DATA_SIZE);
	mvme_write_value(vme,base + V006_CH_0, 0x1);

	return 0;
}

int v006_readLAM(MVME_INTERFACE *vme, mvme_addr_t base, uint16_t ch)
{
	mvme_set_am(vme, V006_AM);
	mvme_set_dmode(vme, V006_DATA_SIZE);
	unsigned short int reg = mvme_read_value(vme,base + 2*ch);

	return (reg >>15);
}
uint32_t v006_adc(MVME_INTERFACE *vme, mvme_addr_t base, uint16_t ch)
{
	mvme_set_am(vme, V006_AM);
	mvme_set_dmode(vme, V006_DATA_SIZE);
	unsigned short int reg = mvme_read_value(vme,base + 2*ch);

	if (reg >>15) 
	return (reg&V006_DATA_MASK);
	else
		return 0;
}


#define TEST_V006
#ifdef TEST_V006
#define V006_BASE 0xEFFF00

#include "TH1F.h"
#include "TCanvas.h"
#include "TApplication.h"
int main(int argc, char **argv) 
{
	printf("Test program for V006: \n");
	MVME_INTERFACE *myvme;
	mvme_open(&myvme, 0);

	v006_clear(myvme,V006_BASE);
  TApplication* theApp = new TApplication("App", &argc, argv);
	TH1F h[8];
	for (int i = 0; i < 8; i++) 
	{
		char title[10], name[10];
		sprintf(name,"chn%d",i);
		sprintf(title,"chn%d",i);
		h[i] = TH1F(name,title,400,0,400);
	}

	int cnt = 0;
	while (cnt < 1000)
	{
		int lam0 = v006_readLAM(myvme,V006_BASE,0);
		/*printf("LAM chn%d: %d\n",0,lam0);*/
		if(lam0)
		{
			/*printf("data: %d\n",v006_adc(myvme,V006_BASE,0));*/
			for (int i = 0; i < 8; i++) 
			{
				h[i].Fill(v006_adc(myvme,V006_BASE,i));
			}
			v006_clear(myvme,V006_BASE);
			cnt++;
		}
		/*}*/
	}

	TCanvas *c = new TCanvas("V006","V006",500,10,400,800);
	c->Divide(2,4);
	for (int i = 0; i < 8; i++) 
	{
		c->cd(i+1);
		h[i].Draw();
	}

	theApp->Run();

	// Finish
	mvme_close(myvme);
	return 0;
}
#endif

