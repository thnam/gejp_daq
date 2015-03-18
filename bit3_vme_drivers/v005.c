// ****************************************************************************
// *     Filename:  v005.c
// *  Description:  
// *   Created by:  Nam Tran (nam at kuno-g.phys.sci.osaka-u.ac.jp)
// *     Time tag:  2013-05-15 14:54:07
// ****************************************************************************
#include "v005.h"
#include "bt617.h"
int v005_clear(MVME_INTERFACE *vme, mvme_addr_t base)
{
	mvme_set_am(vme, V005_AM);
	mvme_set_dmode(vme, V005_DATA_SIZE);
	mvme_write_value(vme,base + V005_CH_0, 0x1);

	return 0;
}

int v005_readLAM(MVME_INTERFACE *vme, mvme_addr_t base, uint16_t ch)
{
	mvme_set_am(vme, V005_AM);
	mvme_set_dmode(vme, V005_DATA_SIZE);
	unsigned short int reg = mvme_read_value(vme,base + 2*ch);

	return (reg >>15);
}
uint32_t v005_adc(MVME_INTERFACE *vme, mvme_addr_t base, uint16_t ch)
{
	mvme_set_am(vme, V005_AM);
	mvme_set_dmode(vme, V005_DATA_SIZE);
	unsigned short int reg = mvme_read_value(vme,base + 2*ch);

	if (reg >>15) 
		return (reg&V005_DATA_MASK);
	else
		return 0;
}


/*#define TEST_V005*/
#ifdef TEST_V005
/*#define V005_BASE 0xDFFF00*/
#define V005_BASE 0xCFFF00

#include "TH1F.h"
#include "TCanvas.h"
#include "TApplication.h"
int main(int argc, char **argv) 
{
	printf("Test program for V005: \n");
	MVME_INTERFACE *myvme;
	mvme_open(&myvme, 0);

	v005_clear(myvme,V005_BASE);
  TApplication* theApp = new TApplication("App", &argc, argv);
	TH1F h[8];
	for (int i = 0; i < 8; i++) 
	{
		char title[10], name[10];
		sprintf(name,"chn%d",i);
		sprintf(title,"chn%d",i);
		h[i] = TH1F(name,title,1300,0,1300);
	}

	int cnt = 0;
	while (cnt < 1000)
	{
		int lam0 = v005_readLAM(myvme,V005_BASE,0);
		/*printf("LAM chn%d: %d\n",0,lam0);*/
		if(lam0)
		{
			printf("data: %d\n",v005_adc(myvme,V005_BASE,0));
			for (int i = 0; i < 8; i++) 
			{
				h[i].Fill(v005_adc(myvme,V005_BASE,i));
			}
			v005_clear(myvme,V005_BASE);
			cnt++;
		}
		/*}*/
	}

	TCanvas *c = new TCanvas("V005","V005",500,10,400,800);
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

