/*
Run TOSSocket Server in Linux.
It makes file, echo_socket, in Linux cur_dir.

VMware Serial
  Socket Name: /home/tad/Tmp/echo_socket
  Client

See [C:/Doc/Comm.HC.Z,1] Remove.

*/

#include "::/Doc/Comm"

U0 CommTest(I64 port)
{
  CComm *c=CommInit8n1(port,9600);
  I64 b=0;
  while (!ScanChar) {
    CommPutS(port,"Testing");
    while (FifoU8Rem(c->RX_fifo,&b))
      "%c",b;
  }
}

CommTest(1);

