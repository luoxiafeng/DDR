#include <common.h>
#include <rtl.h>
#include <dram.h>
#include <asm/io.h>

extern int dramc_set_para(void);
extern int dramc_set_addr(void);
extern void dramc_set_frequency(void);
extern void dramc_set_timing(void);
extern int dramc_init_memory(int);
extern int dramc_verify_size(void);

/*this debug for alin bus test
 *0x2X200014 bus(x)=X-1 : bus4:0x25200014 bus3:0x24200014 bus2:0x23200014 
 *
 * */
//define BUS_TEST 1

/* init virtual storages */
/* the .data section is store in iROM and need to be 
 * load to iRAM before the whole C programs starting
 * to work
 */
void init_data_section(void)
{
	extern uint8_t _data_load[];
	extern uint8_t _data_start[];
	extern uint8_t _data_len[];

	if(_data_load != _data_start)
	  memcpy(_data_start, _data_load, (size_t) _data_len);
}

int init_dram(void)
{
	int reset_flag = 0;

	dramc_debug("Q3E Bare machine code DDR init\n");

	if(dramc_set_para())
		return 1;
	if(dramc_set_addr())
		return 1;
	dramc_set_frequency();
	dramc_set_timing();
	
	reset_flag = (readl(SYS_RTC_RESETSTS) & RESET_WAKEUP_FLAG);
	if(dramc_init_memory(!!reset_flag)) {
		dramc_debug("memory init error!\n");
		return 1;
	}

	/*  will overwrite soc start addr's instruction
	if(dramc_verify_size()) {
		dramc_debug("memory total size verify error!\n");
		return 1;
	}*/

	if(reset_flag == RESET_WAKEUP_FLAG){
		writel(3, SYS_RTC_RSTCLR);
		__attribute__((noreturn)) void (*wake)(void) = (void *)readl(0x42980000);
		wake();
	}

	return 0;
}

int main(void)
{
#ifdef CONFIG_COMPILE_RTL    
	extern uint8_t __lds_soc_base[];
	extern uint8_t __lds_rtl_case[];
	__attribute__((noreturn)) void (*jd)(void)
		= (void *)__lds_soc_base;
#endif    

	init_data_section();
#if 1
	init_timer();
	init_serial();

#ifdef CONFIG_COMPILE_RTL
	puts_enable(0);
#endif

#ifdef	CONFIG_COMPILE_FPGA
	dramc_debug("\nSoC FPGA Testbench: ROM\n");
#else
	dramc_debug("\nSoC Testbench: ROM\n");
#endif
#endif
	if(init_dram()){
		dramc_debug("RTL dram err\n");
        while(1);
		return -1;
	}else
		dramc_debug("RTL dram OK!\n");

#if BUS_TEST
	int i = 0;
	printf("alin test bus\n");
	writel(0xff0,      0x25200014);  //dma count
	writel(0x40000000, 0x25200018);	 //dma buffer
	writel(0x000000f0, 0x2520001c);	 //dma size 16K
	writel(0x00000020, 0x25200000);	 //dma duty cycle

	//while(i=0; i<100; i++){
	while(1){
		writel(0x12345678, 0x40000000);
		printf("readl 0x40000000 0x%x\n", readl(0x40000000));
		udelay(1000);
	}
#endif
	/* jump to SoC directly if RTL env found */
#ifdef CONFIG_COMPILE_RTL
	if(*(uint32_t *)__lds_rtl_case == RTL_MAGIC)
	  jd();
	else
#endif    
	  for(;;);
}

