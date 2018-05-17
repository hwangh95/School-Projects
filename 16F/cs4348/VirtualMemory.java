// Huidong Hwang
// 12/7/16
// Project Assignment #3

import java.io.*;
import java.util.*;

public class VirtualMemory
{
    int VIRTUAL_SIZE = (int) Math.pow(2,16);
    int PHYSICAL_SIZE = (int) Math.pow(2,14);
    int PAGE_SIZE = (int) Math.pow(2,8);
    int PAGE_NUMBER = VIRTUAL_SIZE / PAGE_SIZE;
    int FRAME_NUMBER = PHYSICAL_SIZE / PAGE_SIZE;

    boolean isLRU = false;

    int[] pageTable = new int[PAGE_NUMBER];
    double[] pagePriority = new double[PAGE_NUMBER];
    double pageMax = Double.POSITIVE_INFINITY;

    int[][] tlb = new int[16][2];
    double[] tlbPriority = new double[16];
    double tlbMax = Double.POSITIVE_INFINITY;
    int[] physMem = new int[PHYSICAL_SIZE];
    int[] hardDrive = new int[VIRTUAL_SIZE];

    int pageFaultCount = 0;
    int fifoIndex = 0;

    public VirtualMemory()
    {
        Arrays.fill(pageTable, -1);
        for (int[] row: tlb)
            Arrays.fill(row, -1);
        Arrays.fill(physMem, -1);

        Arrays.fill(pagePriority,Double.POSITIVE_INFINITY);
        Arrays.fill(tlbPriority,Double.POSITIVE_INFINITY);
    }

    public static void main(String[] args) throws Exception
    {
        Scanner in = new Scanner(new File(args[0]));
        String[] stream = null; 
        VirtualMemory driver = new VirtualMemory();
        String output = "";

        if (args[1].toUpperCase().equals("LRU"))
        {
            driver.isLRU = true;
        }
        else if (args[1].toUpperCase().equals("FIFO"))
        {
            driver.isLRU = false;
        }
        else
        {
            System.out.println("Invalid method");
            return;
        }

        while(in.hasNextLine())
        {
            stream = in.nextLine().split(" ");
            output = driver.addressTranslate(Integer.parseInt(stream[1]));
            int physicalAddress = Integer.parseInt(output.split(" ")[4]);
            if (stream[0].equals("W"))
            {
                driver.write(physicalAddress, Integer.parseInt(stream[2]));
            }
            else 
            {
                output += driver.read(physicalAddress); 
            }
            System.out.println(output);
        }

        System.out.println("\nPage Table:");
        for (int i : driver.pageTable)
        {
            if (i == -1)
            {
                break;
            } 
            System.out.println(i);
        }
        System.out.println("\n\n" + driver.pageFaultCount + " Page Faults");
    }

    public String addressTranslate(int virtualAddress)
    {
        int pageNum = virtualAddress / PAGE_SIZE;
        int offset = virtualAddress % PAGE_SIZE;

        int frameNum = tlbSearch(pageNum);
        int physicalAddress = 0;

        boolean TLB_HIT = true;
        boolean PAGE_FAULT = false;
        boolean SWAP_NEEDED = false;

        int tlbIndex = tlbSearch(pageNum);

        if (tlbIndex != -1)
        {
            frameNum = tlb[tlbIndex][1];
            tlbPriority[tlbIndex] = 0; 
            tlbUpdatePriority();
            pagePriority[pageSearch(pageNum)] = 0;
            pageUpdatePriority();
        }
        else
        {
            TLB_HIT = false;
            int pageIndex = pageSearch(pageNum);
            if (pageIndex != -1)
            {
                pagePriority[pageIndex] = 0;
                pageUpdatePriority();
            }
            else
            {
                PAGE_FAULT = true;
                pageFaultCount++;
                
                if (isLRU)
                {
                    for (int i = 0; i < pagePriority.length; i++)
                    {
                        if (pagePriority[i] == pageMax)
                        {
                            pageIndex = i;
                            pageTable[pageIndex] = pageNum;
                            pagePriority[pageIndex] = 0;
                            pageUpdatePriority();
                            break; 
                        }
                    }
                }
                else
                {
                    pageIndex = fifoIndex;
                    pageTable[pageIndex] = pageNum;
                    fifoIndex = (fifoIndex+1) % PAGE_NUMBER;
                }
            }
            
            frameNum = pageIndex * PAGE_SIZE;

            for (int i = 0; i < tlb.length; i++)
            {
                if (tlbPriority[i] == tlbMax)
                {
                    tlb[i][0] = pageNum;
                    tlb[i][1] = frameNum;
                    tlbPriority[i] = 0;
                    tlbUpdatePriority();
                    break;
                }
            } 
        }
        physicalAddress = frameNum + offset;

        return pageNum + " " + offset + " " + (TLB_HIT? "H " : "N ") + (PAGE_FAULT? "F ":"N ") + physicalAddress + " ";
    }

    public void write(int physicalAddress, int data)
    {
        physMem[physicalAddress] = data;
    }

    public int read(int physicalAddress)
    {
        return physMem[physicalAddress];
    }

    public int pageSearch(int pageNum)
    {
        for (int i = 0; i < pageTable.length; i++)
        {
            if (pageTable[i] == pageNum)
            {
                return i; 
            }
        }
        return -1;
    }

    public int tlbSearch(int pageNum)
    {
        for (int i = 0; i < tlb.length; i++)
        {
            if (tlb[i][0] == pageNum)
            {
               return i;
            }
        } 
        return -1;
    }

    public void pageUpdatePriority()
    {
        if (isLRU)
        {
            pageMax = 0;
            for (int i = 0; i < pagePriority.length; i++)
            {
                if (++pagePriority[i] > pageMax)
                {
                    pageMax = pagePriority[i];
                }
            }
        }
    }
    public void tlbUpdatePriority()
    {
        tlbMax = 0;
        for (int i = 0; i < tlbPriority.length; i++)
        {
            if (++tlbPriority[i] > tlbMax)
            {
                tlbMax = tlbPriority[i];
            }
        }
    }
}
