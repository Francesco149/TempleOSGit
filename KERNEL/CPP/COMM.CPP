//These are untested

#define COM1_BASE	0x3F8
#define COM2_BASE	0x2F8
#define COM3_BASE	0x3E8
#define COM4_BASE	0x2E8

#define UART_THR		0
#define UART_RDR		0
#define UART_BRDL		0
#define UART_IER		1
#define UART_BRDH		1
#define UART_IIR		2
#define UART_LCR		3
#define UART_MCR		4
#define UART_LSR		5
#define UART_MSR		6


DWORD comm_base[5];
comm_base[1]=0x3F8;
comm_base[2]=0x2F8;
comm_base[3]=0x3E8;
comm_base[4]=0x2E8;

byte_fifo comm_fifos[5];
int i;
for (i=0;i<5;i++)
  comm_fifos[i]=NULL;

void init_comm_8n1(DWORD port,DWORD baud)
{
  DWORD old_flags=get_flags,base=comm_base[port];
  cli;
  if (comm_fifos[port])
    free(comm_fifos[port]);
  comm_fifos[port]=new_byte_fifo(256);
  outp(base+UART_LCR,0);     //Set for IER
  outp(base+UART_IER,0);     //DIsable all IRQ
  outp(base+UART_LCR,0x80);  //Enable baud rate control
  outp(base+UART_BRDL,(0x180/(baud/300)) & 0xFF); //LSB
  outp(base+UART_BRDL,(0x180/(baud/300)) / 256);  //MSB
  outp(base+UART_LCR,3);     // 8-none-1

  inp(base+UART_RDR);  //read garbage
  inp(base+UART_LSR);

  outp(base+UART_MCR,4);
  outp(base+UART_IER,0);     //DIsable all IRQ
  outp(base+UART_MCR,0xA);	 //out2 and rts
  outp(0x21,inp(0x21) & (0xFF-0x18)); //Enable 8259 IRQ 3 & 4
  outp(base+UART_IER,1);	 //Enable RX IRQ
  set_flags(old_flags);
}

void comm_busy_putchar(DWORD port,BYTE b)
{
  DWORD base=comm_base[port];
  while (!(inp(base+UART_LSR) & 0x20))
    swap_in_next;
  outp(base+UART_THR,b);
  while (!(inp(base+UART_LSR) & 0x20))
    swap_in_next;
}

void comm_busy_puts(DWORD port,char *st)
{
  DWORD base=comm_base[port];
  BYTE b;
  while (b=*(st++)) {
    while (!(inp(base+UART_LSR) & 0x20))
      swap_in_next;
    outp(base+UART_THR,b);
  }
  while (!(inp(base+UART_LSR) & 0x20))
    swap_in_next;
}

void comm_busy_write_block(DWORD port,BYTE *buf,DWORD count)
{
  DWORD base=comm_base[port];
  BYTE b;
  while (count--) {
    while (!(inp(base+UART_LSR) & 0x20))
      swap_in_next;
    outp(base+UART_THR,*(buf++));
  }
  while (!(inp(base+UART_LSR) & 0x20))
    swap_in_next;
}

void irq03_handler()
{
  DWORD base=comm_base[2];
  if ((inp(base+UART_IIR) & 0xFE)==8)	//RX IRQ
    byte_fifo_insert(comm_fifos[2],inp(base+UART_RDR));

  base=comm_base[4];
  if ((inp(base+UART_IIR) & 0xFE)==8)	//RX IRQ
    byte_fifo_insert(comm_fifos[4],inp(base+UART_RDR));
  outp(0x20,0x20);
}

void irq04_handler()
{
  DWORD base=comm_base[1];
  if ((inp(base+UART_IIR) & 0xFE)==8)	//RX IRQ
    byte_fifo_insert(comm_fifos[1],inp(base+UART_RDR));

  base=comm_base[3];
  if ((inp(base+UART_IIR) & 0xFE)==8)	//RX IRQ
    byte_fifo_insert(comm_fifos[3],inp(base+UART_RDR));
  outp(0x20,0x20);
}

defext(ext_irq03,&irq03_handler);
defext(ext_irq04,&irq04_handler);



