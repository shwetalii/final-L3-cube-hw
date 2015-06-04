

#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

//defines for the packet type code in an ETHERNET header....
#define E_TYPE_IP (0x0800)    //Ip protocol...
#define E_TYPE_8021Q (0x8100)

int main(int argc, char **argv)
{
  unsigned int pkt_counter=0;
  unsigned long byte_counter=0; //total bytes seen in entire trace
  unsigned long current_counter=0; //current counter
  unsigned long max_volume = 0;  //max value of bytes in one-second interval
  unsigned long c_timestamp=0;

  //temporary packet buffers
  struct pcap_pkthdr header; // The header that pcap gives us
  const u_char *packet; // The actual packet

  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s [input pcaps]\n", argv[0]);
    exit(1);
  }

  //Main Packet Processing Loop starts....
  for (int fnum=1; fnum < argc; fnum++)
  {


    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE]; //not sure what to do with this, oh well
    handle = pcap_open_offline(argv[fnum], errbuf);   //call pcap library function for opening picap file...

    if (handle == NULL)
        {
          fprintf(stderr,"unable to open pcap file %s: %s\n", argv[fnum], errbuf);
          return(2);
        }

    // in this particular file,processing a packet starts...one at a time...

    while (packet = pcap_next(handle,&header))
    {
      // header contains main information about the packet (e.g. timestamp)
      u_char *packet_ptr = (u_char *)packet; //casting of  a pointer to the packet data

      //parse the first (ethernet) header, grabbing the type field
      int ether_type = ((int)(packet_ptr[12]) << 8) | (int)packet_ptr[13];
      int ether_offset = 0;

      if (ether_type == E_TYPE_IP) //most common
        ether_offset = 14;
      else if (ether_type == E_TYPE_8021Q) //my traces have this
         ether_offset = 18;
      else
         fprintf(stderr, "Unknown ethernet type, %04X, skipping...\n", ether_type);

      //now it's time to parse the IP header...
      packet_ptr += ether_offset;  //skip past the Ethernet II header
      struct ip *ip_hdr = (struct ip *)packet_ptr; //now point to an IP header structure

      int length_of_packet = ntohs(ip_hdr->ip_len);///The ntohs function converts a u_short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors).

      //check to see if the next second has started, for statistics purposes
      if (c_timestamp == 0)//this takes care of the very first packet seen
      {
         c_timestamp = header.ts.tv_sec;
      }
      else if (header.ts.tv_sec > c_timestamp)
      {
         printf("%d Kbps\n", current_counter/1000); //print it....
         current_counter = 0; //reset counters
         c_timestamp = header.ts.tv_sec; //update time interval
      }

      current_counter += length_of_packet;
      byte_counter += length_of_packet; //update byte counter............
      pkt_counter++; //increment number of total packets seen..........

    } //end internal loop for reading packets (all in one file)

    pcap_close(handle);  //close the pcap file........

  }
  //end with main packet Processing Loop

  //now output some statistics about the whole trace.
  byte_counter /= 1e6;  //converting  to MB to make easier to read.

  printf("Processed %d packets and %u MBytes, in %d files\n", pkt_counter, byte_counter, argc-1);
  return 0;
}
