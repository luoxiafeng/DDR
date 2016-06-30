#include <dram.h>
#ifdef CONFIG_COMPILE_RTL
#include "dram-reg.h"
#endif
#include "dram-reg.h"
#ifdef CONFIG_COMPILE_FPGA
#include "dram-reg-fpga.h"
#endif
#include <asm/io.h>
#ifndef read_trans_send
#define read_trans_send(A,B)  (B) = readl(A)
#endif
#ifndef writel_reverse
#define writel_reverse(A,B) writel(B,A)
#endif
//400MHZ
//ivan 0620
#define      DDR_TRAIN_TEST
void test_umctl_reg_ddr2(void)
{
    unsigned int read_tmp;

    writel_reverse(ADDR_UMCTL2_DBG1          ,0x00000001);
    writel_reverse(ADDR_UMCTL2_PWRCTL        ,0x00000001);
    read_trans_send(ADDR_UMCTL2_STAT,read_tmp);
    //DDR2,[0] ddr3, 0 non-DDR3 SDRAM
    writel_reverse(ADDR_UMCTL2_MSTR          ,0x83040000);  //bit[0]--ddr3,bit[9]--burstchop(off),bit[10]--en_2t_timing_mode(1T),bit[13:12]--data_bus_width(full),bit[15]--dll_off_mode(normal),bit[19:16]--burst_rdwr(BL8),bit[27:24]--active_banks(two banks),bit[29]--frequences_mode(original)
    read_trans_send(ADDR_UMCTL2_MSTR,read_tmp);
    //
    writel_reverse(ADDR_UMCTL2_MRCTRL0       ,0x00003030);
    writel_reverse(ADDR_UMCTL2_MRCTRL1       ,0x00004daf); //bit[17:0]--mr_data
    //lpddr2 only
    writel_reverse(ADDR_UMCTL2_DERATEEN      ,0x00000020); //bit[0]--derate_enable(0),bit[1]--derate_value(0),bit[7:4]--derate_type(2),not used for ddr3
    //lpddr2 only
    writel_reverse(ADDR_UMCTL2_DERATEINT     ,0xa1ca5c09); //bit[31:0]--mr4_read_interval(),,not used for ddr3
    writel_reverse(ADDR_UMCTL2_PWRCTL        ,0x0000000a); //bit[0]--selfresh_en(0),bit[1]--powerdown_en(1),bit[2]--deeppowerdown_en(0),bit[3]--en_dfi_dram_clk_disable(1),bit[5]--selfreh_sw(0),
    //[23:16] selfref_to_x32, [4:0] powerdown_to_x32, ?
    writel_reverse(ADDR_UMCTL2_PWRTMG        ,0x00270203); //bit[4:0]--powerdown_to_x32(2),bit[15:8]--t_dpt_x4096('h32),bit[23:16]--selfref_to_x32('he)
    writel_reverse(ADDR_UMCTL2_HWLPCTL       ,0x00810001);
    //[23:20] refresh_margin 0x2, [16:12] refresh_to_x32,[8:4] refresh_burst, [2] per_bank_refresh ,lpddr only?
    //writel_reverse(ADDR_UMCTL2_RFSHCTL0    ,0x00202010); //bit[2]--per_bank_refresh(0),bit[8:4]--refresh_burst(7),bit[16:12]--refresh_to_x32('h1c),bit[23:20]--refresh_margin(2),800
    writel_reverse(ADDR_UMCTL2_RFSHCTL0      ,0x00202010); //bit[2]--per_bank_refresh(0),bit[8:4]--refresh_burst(7),bit[16:12]--refresh_to_x32('h1c),bit[23:20]--refresh_margin(2),1066
    writel_reverse(ADDR_UMCTL2_RFSHCTL1      ,0x00200009);
    writel_reverse(ADDR_UMCTL2_RFSHCTL3      ,0x00000000); //bit[0]--dis_auto_refresh(0),bit[1]--refresh_updata_level(0) 
    //writel_reverse(ADDR_UMCTL2_RFSHTMG     ,0x0030003c);//RFSHTMG,800
    //[9:0] t_rfc_min,56ck,'h38
    writel_reverse(ADDR_UMCTL2_RFSHTMG       ,0x00300015);//RFSHTMG,1066
    writel_reverse(ADDR_UMCTL2_CRCPARCTL0    ,0x00000000); //bit[0]--dfi_alert_err_int_en(0),bit[1]--dfi_alert_err_int_clr(0),bit[2]--dfi_alert_err_cnt_clr(1)
    //[31:30] skip_dram_init,[25:16] post_cke_x1024, [11:0] pre_cke_x1024
    //writel_reverse(ADDR_UMCTL2_INIT0         ,0x400200c4);
    //[25:16] post_cke_x1024,400ns,ddr2,2,[11:0] pre_cke_x1024,200us, 53,'h35
    writel_reverse(ADDR_UMCTL2_INIT0         ,0x40020028);//[31:30] must be 2'b01
    //[24:16] dram_rstnx1024,Synopsys DDR PHY, 1,[14:8] 0x0
    //writel_reverse(ADDR_UMCTL2_INIT1         ,0x004f0000);
    //[14:8] final_wait_x32
    writel_reverse(ADDR_UMCTL2_INIT1         ,0x00690700);
    //lpddr2 only
    writel_reverse(ADDR_UMCTL2_INIT2         ,0x00005208);
    //[31:16] mode register0, [15:0] mode register 1
    //writel_reverse(ADDR_UMCTL2_INIT3         ,0x05200046);
    //[31:16] mr,[15:0] emr
    writel_reverse(ADDR_UMCTL2_INIT3         ,0x0b630000);
    //[31:16] mode register2,emr2, [15:0] mode register 3,emr3
    writel_reverse(ADDR_UMCTL2_INIT4         ,0x00080000); //bit[15:0]--emr3,bit[31:16]--emr2
    //[23:16] dev_zqinit_x32, =512/32,[9:0] lpddr2
    writel_reverse(ADDR_UMCTL2_INIT5         ,0x00100000); //bit[9:0]--max_auto_init_x1024,bit[23:16]--dev_zqinit_x32,512 cycles
    writel_reverse(ADDR_UMCTL2_DIMMCTL       ,0x00000000); //bit[0]--dimm_stagger_cs_en,bit[1]--dimm_addr_mirr_en
    //under register not assure
    //[11:8] diff_rank_wr_gap, [7:4] diff_rank_rd_gap, [3:0] max_rank_rd
    writel_reverse(ADDR_UMCTL2_RANKCTL       ,0x00000b4f);//?
    //writel_reverse(ADDR_UMCTL2_DRAMTMG0      ,0x080a0e0a);//MEMC_FREQ_RATIO=2, [30:24] wr2pre= 8cycle + 15ns/2, [21:16] x16 tFAW=50ns(800)/2,[14:8] (t_ras_max(9xtREFI)-1)/2,tREFI=7.8us, [5:0] t_ras_min=37.5(800)/2
    //writel_reverse(ADDR_UMCTL2_DRAMTMG1      ,0x0005020e);//[20:16] t_xp/2, MR0[12] == 1'b0 ? tXPDLL(greater of 10tCK or 24ns) : tXP,[12:8] rd2pre tRTP = tAL+max(tRTP,4)(DDR3)/2,MEMC_FREQ_RATIO=2,[6:0] t_rc/2= 
    //writel_reverse(ADDR_UMCTL2_DRAMTMG2      ,0x03030407);//[29:24], WL, mDDR,LPDDR2, DDR4 only,[21:16] RL/2 mDDR,LPDDR2, DDR4 only,[13:8] RL+BL/2+2-WL/2,[5:0] (CWL+BL/2+tWTR)/2
    //writel_reverse(ADDR_UMCTL2_DRAMTMG3      ,0x00002006);//[29:20] t_mrw, lpddr2 only, [17:12] tMRD/2, [9:0] tMOD/2
    //writel_reverse(ADDR_UMCTL2_DRAMTMG4      ,0x04020304);//[28:24] tRCD-tAL/2, [19:16] tCCD/2, [11:8] tRRD/2,[4:0] (RoundUp(tRP/tCK)/2)+1
    //writel_reverse(ADDR_UMCTL2_DRAMTMG5      ,0x03030202);//[27:24] tCKSRX/2,[19:16] max(10ns, 5tCK)/2, [13:8] (tCKE+1)/2,[4:0] tCKE/2
    //[5:0] t_ras_min,[14:8] t_ras_max,[21:16] t_faw
    writel_reverse(ADDR_UMCTL2_DRAMTMG0      ,0x08090d09);//MEMC_FREQ_RATIO=2, [30:24] wr2pre= 8cycle + 15ns/2, [21:16] x16 tFAW=50ns(800)/2,[14:8] (t_ras_max(9xtREFI)-1)/2,tREFI=7.8us, [5:0] t_ras_min=37.5(800)/2
    //[6:0] t_rc 60ns(800),24ck
    writel_reverse(ADDR_UMCTL2_DRAMTMG1      ,0x0001020c);//[20:16] t_xp/2, MR0[12] == 1'b0 ? tXPDLL(greater of 10tCK or 24ns) : tXP,[12:8] rd2pre tRTP = tAL+max(tRTP,4)(DDR3)/2,MEMC_FREQ_RATIO=2,[6:0] t_rc/2= 
    writel_reverse(ADDR_UMCTL2_DRAMTMG2      ,0x03040406);//[29:24], WL, mDDR,LPDDR2, DDR4 only,[21:16] RL/2 mDDR,LPDDR2, DDR4 only,[13:8] RL+BL/2+2-WL/2,[5:0] (CWL+BL/2+tWTR)/2
    //[17:12] t_mrd
    writel_reverse(ADDR_UMCTL2_DRAMTMG3      ,0x00001000);//[29:20] t_mrw, lpddr2 only, [17:12] tMRD/2, [9:0] tMOD/2
writel_reverse(ADDR_UMCTL2_DRAMTMG4      ,0x03020204);//[28:24] tRCD-tAL/2, [19:16] tCCD/2, [11:8] tRRD/2,[4:0] (RoundUp(tRP/tCK)/2)+1
    writel_reverse(ADDR_UMCTL2_DRAMTMG5      ,0x01010202);//[27:24] tCKSRX/2,[19:16] max(10ns, 5tCK)/2, [13:8] (tCKE+1)/2,[4:0] tCKE/2
    writel_reverse(ADDR_UMCTL2_DRAMTMG6      ,0x09090000);//[27:24] mddr,lpddr only,[19:16] mddr,lpddr only,[3:0] mddr,lpddr only
    writel_reverse(ADDR_UMCTL2_DRAMTMG7      ,0x00000509);//[11:8] mddr,lpddr only, [3:0] mddr, lpddr only
    //writel_reverse(ADDR_UMCTL2_DRAMTMG8    ,0x03030802);//[30:24] ddr4 only,[22:16] ddr4 only,bit[6:0]--t_xs_x32=tXS/2,bit[14:8]--tXSDLL/2
    writel_reverse(ADDR_UMCTL2_DRAMTMG8      ,0x03030803);//[30:24] ddr4 only,[22:16] ddr4 only,bit[6:0]--t_xs_x32=tXS/2,bit[14:8]--tXSDLL/2
    writel_reverse(ADDR_UMCTL2_DRAMTMG14     ,0x00000cc9);//[11:0] mddr,lpddr only
    //[31:16] mr,[15:0] emr
    writel_reverse(ADDR_UMCTL2_INIT3         ,0x0a630000);
    writel_reverse(ADDR_UMCTL2_ZQCTL0        ,0x60800020);
    writel_reverse(ADDR_UMCTL2_ZQCTL1        ,0x01400000);
    writel_reverse(ADDR_UMCTL2_ZQCTL2        ,0x00000000); //bit[0]--zq_reset
    writel_reverse(ADDR_UMCTL2_DFITMG0       ,0x02020002);
    writel_reverse(ADDR_UMCTL2_DFITMG1       ,0x1100c202);
    writel_reverse(ADDR_UMCTL2_DFILPCFG0     ,0x07901110);
    writel_reverse(ADDR_UMCTL2_DFIUPD0       ,0x00400004); //bit[9:0]--dfi_t_ctrlup_min,bit[25:16]--dfi_t_ctrlup_max,bit[30]--dis_auto_ctrlupd_srx,bit[31]--dis_auto_ctrlupd
    writel_reverse(ADDR_UMCTL2_DFIUPD1       ,0x002900a7);
    writel_reverse(ADDR_UMCTL2_DFIUPD2       ,0x00000000); //bit[31]--dfi_phyupd_en
    writel_reverse(ADDR_UMCTL2_DFIMISC       ,0x00000001); //add new
    //[20:16] addrmap_dch_bit0, [12:8] addrmap_cs_bit1, [4:0] addrmap_cs_bit0
    writel_reverse(ADDR_UMCTL2_ADDRMAP0      ,0x001f1f1f); //bit[4:0]--addrmap_cs_bit0
    //[20:16] addrmap_bank_b2,[12:8] addrmap_bank_b1,[4:0] addrmap_bank_b0
    //512Mb, b2 not used
    writel_reverse(ADDR_UMCTL2_ADDRMAP1      ,0x001f0808);
    //[27:24] addrmap_col_b5,[19:16] addrmap_col_b4, [11:8] addrmap_col_b3, [3:0] addrmap_col_b2
    writel_reverse(ADDR_UMCTL2_ADDRMAP2      ,0x00000000);
    //[27:24] addrmap_col_b9,[19:16] addrmap_col_b8,[11:8] addrmap_col_b7, [3:0] addrmap_col_b6
    writel_reverse(ADDR_UMCTL2_ADDRMAP3      ,0x00000000);
    //[11:8] addrmap_col_b11,[3:0] addrmap_col_b10
    writel_reverse(ADDR_UMCTL2_ADDRMAP4      ,0x00000f0f);
    //[27:24] addrmap_row_b11,[19:16] addrmap_row_b2_10,[11:8] addrmap_row_b1,[3:0] addrmap_row_b0
    writel_reverse(ADDR_UMCTL2_ADDRMAP5      ,0x06060606);
    //[31] lpddr3_6gb_12gb, [27:24] addrmap_row_b15,[19:16] addrmap_row_b14,[11:8] addrmap_row_b13, [3:0] addrmap_row_b12
    //512Mb = row_address[12:0]
   writel_reverse(ADDR_UMCTL2_ADDRMAP6      ,0x0f0f0f06);
    //[27:24] addrmap_row_b5, [19:16] addrmap_row_b4,[11:8] addrmap_row_b3,[3:0] addrmap_row_b2
    //writel_reverse(ADDR_UMCTL2_ADDRMAP9      ,0x06060606);
    //[27:24] addrmap_row_b9,[19:16] addrmap_row_b8,[11:8] addrmap_row_b7, [3:0] addrmap_row_b6
    //writel_reverse(ADDR_UMCTL2_ADDRMAP10     ,0x0);
    //[27:24] addrmap_row_b10,lpddr only
    //writel_reverse(ADDR_UMCTL2_ADDRMAP11     ,0x0); //bit[3:0]--addrmap_row_b10
writel_reverse(ADDR_UMCTL2_ODTCFG        ,0x06000604);
    writel_reverse(ADDR_UMCTL2_ODTMAP        ,0x00000000);
    //[2] pageclose,1 bank is kept open 
    writel_reverse(ADDR_UMCTL2_SCHED         ,0x00861f01);//[30:24]rdwr_idle_gap,[1]prefer_write
    //writel_reverse(ADDR_UMCTL2_SCHED1      ,0x00000006); //bit[7:0]--pageclose_timer
    writel_reverse(ADDR_UMCTL2_SCHED1        ,0x00000000); //bit[7:0]--pageclose_timer
    writel_reverse(ADDR_UMCTL2_PERFHPR1      ,0x040000c0); //bit[15:0]--hpr_max_starve,bit[31:24]--hpr_xact_run_length    
    writel_reverse(ADDR_UMCTL2_PERFLPR1      ,0x04000045);
    writel_reverse(ADDR_UMCTL2_PERFWR1       ,0x040000c0);
    writel_reverse(ADDR_UMCTL2_PERFVPR1      ,0x000007ff); //bit[10:0]--vpr_timeout_range  //not use
    writel_reverse(ADDR_UMCTL2_PERFVPW1      ,0x000007ff); //bit[10:0]--vpw_timeout_range  //not use
writel_reverse(ADDR_UMCTL2_DBG0          ,0x00000016);
    writel_reverse(ADDR_UMCTL2_DBG1          ,0x00000000); //bit[0]--dis_dq,bit[1]--dis_hif
    writel_reverse(ADDR_UMCTL2_DBGCMD        ,0x00000000); //bit[0]--rank0_refresh,bit[1]--rank1_refresh,bit[2]--rank2_refresh,bit[3]--rank3_refresh,bit[4]--zq_calib_short,bit[5]--ctrlupd
    writel_reverse(ADDR_UMCTL2_SWCTL         ,0x00000001); //bit[0]--sw_done
    //
    writel_reverse(ADDR_UMCTL2_PCCFG         ,0x00000010); //bit[]--,bit[]--,bit[]--
    //[12] rd_port_aging_en, [9:0] rd_port_priority
    writel_reverse(ADDR_UMCTL2_PCFGR_0       ,0x0000500f); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGW_0       ,0x00004080); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGQOS0_0    ,0x02100053); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGQOS1_0    ,0x00ff00ff); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGWQOS0_0   ,0x00100005); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGWQOS1_0   ,0x000000ff); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGR_1       ,0x00005001); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGW_1       ,0x00004080); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGQOS0_1    ,0x02100053); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGQOS1_1    ,0x00ff00ff); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGWQOS0_1   ,0x00100005); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGWQOS1_1   ,0x000000ff); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGR_2       ,0x00005001); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGW_2       ,0x00004080); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCTRL_2       ,0x00000001); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGQOS0_2    ,0x02100053); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGQOS1_2    ,0x00ff00ff); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGWQOS0_2   ,0x00100005); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGWQOS1_2   ,0x000000ff); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGR_3       ,0x0000500f); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGW_3       ,0x00004080); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGQOS0_3    ,0x02100053); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGQOS1_3    ,0x00ff00ff); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGWQOS0_3   ,0x00100005); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGWQOS1_3   ,0x000000ff); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PCFGR_4       ,0x0000500f);//[9:0] rd_port_priority
    writel_reverse(ADDR_UMCTL2_PCFGW_4       ,0x00004080);//[9:0] wr_port_priority
    writel_reverse(ADDR_UMCTL2_PCTRL_4       ,0x00000001);
    writel_reverse(ADDR_UMCTL2_PCFGQOS0_4    ,0x02100053);
    writel_reverse(ADDR_UMCTL2_PCFGQOS1_4    ,0x00ff00ff);
    writel_reverse(ADDR_UMCTL2_PCFGWQOS0_4   ,0x00100005);
    writel_reverse(ADDR_UMCTL2_PCFGWQOS1_4   ,0x000000ff);
    writel_reverse(ADDR_UMCTL2_PCTRL_0       ,0x00000001);
    writel_reverse(ADDR_UMCTL2_PCTRL_1       ,0x00000001);
    writel_reverse(ADDR_UMCTL2_PCTRL_2       ,0x00000001);
    writel_reverse(ADDR_UMCTL2_PCTRL_3       ,0x00000001);
    writel_reverse(ADDR_UMCTL2_PCTRL_4       ,0x00000001); //bit[]--,bit[]--,bit[]--
    read_trans_send(ADDR_UMCTL2_RFSHCTL3      ,read_tmp);
    //repeat(155) 
     // @apb_dut_master_if.pclk;
    writel_reverse(ADDR_UMCTL2_RFSHCTL3,0x00000000);
    writel_reverse(ADDR_UMCTL2_DBG1          ,0x00000000); //bit[]--,bit[]--,bit[]--
    read_trans_send(ADDR_UMCTL2_PWRCTL        ,read_tmp); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_PWRCTL        ,0x00000008);
    read_trans_send(ADDR_UMCTL2_PWRCTL        ,read_tmp);
    writel_reverse(ADDR_UMCTL2_PWRCTL        ,0x00000008);
    //repeat(1000) 
     // @apb_dut_master_if.pclk;    
    writel_reverse(ADDR_UMCTL2_SWCTL         ,0x00000000); //bit[]--,bit[]--,bit[]--
    writel_reverse(ADDR_UMCTL2_DFIMISC       ,0x00000000); //bit[]--,bit[]--,bit[]--
}


void test_phy_reg_ddr2(void)                                                      
{              
    unsigned int read_tmp;

    writel(0xb00001ce, ADDR_PHY_ZQ0CR0);
    writel(0xb00001ce, ADDR_PHY_ZQ1CR0);
    writel(0x0, ADDR_PHY_ZQ0CR1);
    writel(0x0, ADDR_PHY_ZQ1CR1);
    #ifdef DDR_TRAIN_TEST
    // ivan 0620
    //writel_reverse(ZQ0CR1, 0x0b);
    //writel_reverse(ZQ1CR1, 0x0b);
    #endif
    //read_trans_send(0x1234_5678,read_tmp);
    //read_trans_send(0x1234_5678,read_tmp);
read_trans_send(ADDR_UMCTL2_STAT,read_tmp); 
    //repeat(20) 
     // @apb_dut_master_if.pclk;

    read_trans_send( ADDR_PHY_PGCR, read_tmp );
    writel_reverse(ADDR_PHY_DX2GCR, 0x1b6cce80);
    writel_reverse(ADDR_PHY_DX3GCR, 0x1b6cce80);
    writel_reverse(ADDR_PHY_PGCR, 0x1842e06  );
    writel_reverse(ADDR_PHY_PGCR, 0x1842e04  );// ivan 0620
    //[2:0] DDRMD,ddr2=010
    writel_reverse(ADDR_PHY_DCR, 0x0000000a  );
    //writel_reverse(ADDR_PHY_MR0, 0x00000510  );
    //writel_reverse(ADDR_PHY_MR0, 0x00000520  );//800
    //writel_reverse(ADDR_PHY_MR1, 0x00000010  );
    //writel_reverse(ADDR_PHY_MR2, 0x00000000  );//800
    //EMR2,[2:0] PASR,
    writel_reverse(ADDR_PHY_MR2, 0x00000008  );//1066,wr=8
    writel_reverse(ADDR_PHY_MR3, 0x00000000  );
    //[9:7] OCD,[5:3] AL=0, [6,2] RTT,[1] DIC, [0] DE
    writel_reverse(ADDR_PHY_MR1, 0x00000000  );
    read_trans_send( ADDR_PHY_MR1, read_tmp );
    //[2:0] BL,[3] BT, [6:4] CL, 7,[7] TM,0, [8] DR(DLL Reset), [11:9] WR, [12] PD
    writel_reverse(ADDR_PHY_MR0, 0x00000b63  );//1066,cl=7,wr=8
    read_trans_send( ADDR_PHY_MR0, read_tmp );
    writel_reverse(ADDR_PHY_MR0, 0x00000a63  );//1066,cl=7,wr=8
    read_trans_send( ADDR_PHY_MR0, read_tmp );
    //[9:7] OCD 000 OCD calibration mode exit, 111 OCD calibration default,[5:3] AL=0, [6,2] RTT,[1] DIC, [0] DE
    //[9:7] 'b111 OCD calibration default
    writel_reverse(ADDR_PHY_MR1, 0x00000380  );
    read_trans_send( ADDR_PHY_MR1, read_tmp );
    //[9:7] 'b000 OCD calibration mode exit
    writel_reverse(ADDR_PHY_MR1, 0x00000000  );
    read_trans_send( ADDR_PHY_MR1, read_tmp );   //
//writel_reverse(ADDR_PHY_DTPR0, 0x2a8f6693);//800
    //[1:0] tMRD=2(DDR2),[4:2] tRTP 6,[7:5] tWTR 4,
    //[1:0] tMRD 2,[4:2] tRTP 6,[7:5] tWTR, [11:8] tRP, [15:12] tRCD,6(800), [20:16] tRAS 'h18, [24:21] tRRD 6, [30:25] tRC 'h1f,[31] tCCD
    writel_reverse(ADDR_PHY_DTPR0, 0x3092666e);//1066
    //writel_reverse(ADDR_PHY_DTPR1, 0x007800a0);//800
    //[1:0] tAOND/tAOFD(DDR2 only)
    //[29:27] tDQSCKmax(lpddr2),[26:24] tDQSCK(lpddr2),[23:16] tRFC,[11]tRTODT, [8:3] tFAW 'h18,[1:0] 00
    writel_reverse(ADDR_PHY_DTPR1, 0x002a0090);
    read_trans_send(ADDR_PHY_DTPR2, read_tmp ); 
    //writel_reverse(ADDR_PHY_DTPR2, 0x1001b00c);//800
    //[9:0] DTPR2 tXS = max(tXSNR,tXSRD) DDR2,[14:10]tXP = max(tXP,tXARD,tXARDS) DDR2
    //[28:19],tDLLK,200CK='hc8, [18:15] tCKE 3, [14:10] tXP=3,tXARD=3,tXARDS=10-AL(AL=0),tXP=max(tXP,tXARD,tXARDS) 10='ha, [9:0] tXS =max(tXSNR=tRFC+10,tXSRD=200CK)=200ck='hc8;
    writel_reverse(ADDR_PHY_DTPR2,   0x0641a0c8);//1066
    writel_reverse(ADDR_PHY_PTR0, 0x0020051b );//apb clock
    read_trans_send(ADDR_PHY_DSGCR, read_tmp ); 
    writel_reverse(ADDR_PHY_DSGCR, 0xfa00001f); 
    //PTR1,[18:0] tDINIT0(ddr2=200us,19'h1a0aa), [26:19] tDINIT1(ddr2=400ns),8'hd6
    writel_reverse(ADDR_PHY_PTR1, 0x05013880 );//sdram clock,[26:19] ='h90,(tDINIT1,tRFC+10ns) [18:0]='h411ab(tDINIT0,500us) 
    //[16:0] tDINIT2, DDR3=200us
    //writel_reverse(ADDR_PHY_PTR2, 0x03213880 );//sdram clock,800
    writel_reverse(ADDR_PHY_PTR2, 0x01a8c8 );//sdram clock,1066,[16:0],tDINIT2,200us
do
      read_trans_send(ADDR_PHY_PGSR,read_tmp);
    while((read_tmp & 0x00000003) != 0x00000003);//step 4 
    //while((read_tmp & 0x00000007) != 0x00000007);//step 4 
    //sdram initialization
    writel_reverse(           ADDR_PHY_PIR, 0x00000061 );//step 5
    do
      read_trans_send(ADDR_PHY_PGSR,read_tmp);
    while((read_tmp & 0x00000003) != 0x00000003);
    //while((read_tmp & 0x0000000f) != 0x0000000f);ivan
  #ifndef BISTLP_TEST
  //#if 1
writel_reverse(ADDR_UMCTL2_SWCTL, 0x00000000); 
    writel_reverse(ADDR_UMCTL2_DFIMISC, 0x00000001);//0x1b0
    writel_reverse(ADDR_UMCTL2_SWCTL, 0x00000001);
    do
      read_trans_send(ADDR_UMCTL2_SWSTAT,read_tmp);
    while((read_tmp &0x1) != 1);
    do 
      read_trans_send(ADDR_UMCTL2_STAT,read_tmp);
    while((read_tmp&0x3) != 1);//setp 10
    //repeat(102) 
     // @apb_dut_master_if.pclk;    
    writel_reverse(ADDR_UMCTL2_RFSHCTL3, 0x00000001);//0x60
    read_trans_send(ADDR_UMCTL2_PWRCTL,read_tmp);
    writel_reverse(ADDR_UMCTL2_PWRCTL, read_tmp & 0xfffffffd);
    read_trans_send(ADDR_UMCTL2_DFIMISC,read_tmp);
    writel_reverse(ADDR_UMCTL2_DFIMISC, read_tmp & 0xfffffffe);
    read_trans_send(ADDR_PHY_PGCR,read_tmp);
    writel_reverse(ADDR_PHY_PGCR,read_tmp | 0x02000006  );
    writel_reverse(ADDR_PHY_PGCR,read_tmp | 0x02000004  ); // ivan ,0620
    //DXnDQSTR
    //writel_reverse(ADDR_PHY_DX0DQSTR, 0x7FFFF000 );
    //writel_reverse(ADDR_PHY_DX1DQSTR, 0x7FFFF000 );
    //DXnGCR[DXEN]
    //PGCR[RANKEN]
    //precharge all banks, MPR
    //enable MPR
    //DTAR
    //address,data,DTAR,DTDTR0,DTDTR1
    //
    //writel_reverse(ADDR_PHY_DTAR, 0x7FFFF000 );
    writel_reverse(ADDR_PHY_DTAR, 0x40000000 );
    //[7]Read DQS Training, [8]Read Valid Training
    //writel_reverse(ADDR_PHY_PIR, 0x00000181 );
    writel_reverse(ADDR_PHY_PIR, 0x40000181 );
    do
      read_trans_send(ADDR_PHY_PGSR,read_tmp);
    while((read_tmp & 0x000003fb)!= 0x0000001b);//step 4
    //while((read_tmp & 0x000003ff)!= 0x0000001f);//step 4ivan
    writel_reverse(ADDR_UMCTL2_SWCTL         ,0x00000001);
    writel_reverse(ADDR_UMCTL2_RFSHCTL3, 0x00000000);//0x60
  #endif    

}
//writel_reverse(D
    //only start axi
//    read_trans_send(0x0000_f000,read_tmp);
//$display("end ddr cfg scenarios,@%0d\n",$time);
 // endtask
 //
 
#if 1  
void test_umctl_reg(void)
{
    test_umctl_reg_ddr2();
}



void test_phy_reg(void)
{
    test_phy_reg_ddr2();
}
#endif
