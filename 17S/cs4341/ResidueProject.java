import java.util.Scanner;
import java.util.Arrays;
import java.text.DecimalFormat;
import java.io.*;

public class ResidueProject
{
    public static void main(String args[]) throws IOException
    {
        Scanner file = new Scanner(new File("input.txt"));
        long range = Long.parseLong(file.nextLine());
        RNS rnsDriver = new RNS(range);
        rnsDriver.printStats();
        while (file.hasNextLine())
        {
            String[] opstream = file.nextLine().split("(?=\\+|\\-|\\*)|(?<=\\+|\\-|\\*)");
            String[] operation = new String[3];
            if (opstream[0].equals("-"))
            {
                operation[0] = "-" + opstream[1];
                operation[1] = opstream[2];
            }
            else
            {
                operation[0] = opstream[0];
                operation[1] = opstream[1];
            }
            if (opstream[opstream.length-3].matches("\\+|\\-|\\*"))
            {
                operation[2] = "-" + opstream[opstream.length-1];
            }
            else
            {
                operation[2] = opstream[opstream.length-1];
            }

            long term1 = Long.parseLong(operation[0]);
            long term2 = Long.parseLong(operation[2]);

            switch (operation[1])
            {
                case "+":
                    rnsDriver.add(term1,term2);
                    break;
                case "-":
                    rnsDriver.sub(term1,term2);
                    break;
                case "*":
                    rnsDriver.mult(term1,term2);
                    break;
            }
        }
    }
}

class RNS
{
    private int[] moduli;
    private int maxWidth;
    private double efficiency;
    private long total = 1;
    
    public RNS(long range)
    {
        setModuli(range);
        calcEfficiency(range);
    }
    
    private void setModuli(long range)
    {
        int p = 3;
        while (total < range)
        {
            total = 1;
            if (p <= 4 || (p % 2) == 0)
                moduli = new int[] {(int)Math.pow(2,p), (int)Math.pow(2,p)-1,
                                    (int)Math.pow(2,p-1)-1};
            //With the given moduli finding scheme, only 3 moduli exist for p = 3,4
            else
                moduli = new int[] {(int)Math.pow(2,p), (int)Math.pow(2,p)-1,
                                    (int)Math.pow(2,p-1)-1, (int)Math.pow(2,p-2)-1};

            for (int i : moduli)
            {
                total *= i;
            }
            maxWidth = p;
            System.out.println(total);
            p++;
        }
    }

    private void calcEfficiency(long range)
    {
        efficiency = (double)range/total *100;
    }

    private int[] convertToRNS(long term)
    {
        int[] rns = new int[moduli.length]; 
        for (int i = 0; i < rns.length; i++)
        {
            rns[i] = (int)(term % moduli[i] + moduli[i]) % moduli[i];
            //Accounts for negative terms
        }
        return rns;
    }

    private long convertToDec(int[] rns)
    {
        long dec = 0;
        for (int i = 0; i < rns.length; i++)
        {
            dec += (rns[i]*multInverse(moduli[i]) % moduli[i])*(total/moduli[i]);
        }
        dec = dec % total;
        while (dec < -total/2 || dec > total/2 - 1)
        {
            if (dec < -total/2)
                dec = (dec + total) % total;
            else
                dec = (dec - total) % total;
        }
        return dec;
    }
    
    private long multInverse(long num)
    {
        long i,p2,p,q2,q1,q;
        i = p2 = p = q2 = q1 = q = 0;
        long p1 = 1;
        long x = total/num;
        long n = num;
        long r = num;
        while (r!=0)
        {
            q = n/x;
            r = n % x; 
            n = x;
            x = r;

            q2 = q1;
            q1 = q;

            if (i>0)
            {
                p = (p2-p1*q2);
                p2 = p1;
                p1 = p;
            }
            i++;
        }
        return p;
    }

    private String stringBuilder(long term1, long term2, int[] rns1, int[] rns2, int[] result, String op)
    {
        String rns1str = Arrays.toString(rns1);
        String rns2str = Arrays.toString(rns2);
        String resultstr = Arrays.toString(result);

        String output = "";
        output +=  term1+" "+op+" "+term2+" = ";
        output += "RNS("+rns1str.substring(1,rns1str.length()-1)+") "+op+" ";
        output += "RNS("+rns2str.substring(1,rns2str.length()-1)+") = ";
        output += "RNS("+resultstr.substring(1,resultstr.length()-1)+") = ";
        output += convertToDec(result);

        return output;
    }

    public void add(long term1, long term2)
    {
        int[] rns1 = convertToRNS(term1);
        int[] rns2 = convertToRNS(term2);
        int[] result = new int[moduli.length];

        for (int i = 0; i < result.length; i++)
        {
            result[i] = (rns1[i]+rns2[i]) % moduli[i];
        }

        System.out.println(stringBuilder(term1, term2, rns1, rns2, result,"+"));
    }

    public void sub(long term1, long term2)
    {
        int[] rns1 = convertToRNS(term1);
        int[] rns2 = convertToRNS(term2);
        int[] result = new int[moduli.length];

        for (int i = 0; i < result.length; i++)
        {
            result[i] = (rns1[i]-rns2[i] + moduli[i]) % moduli[i];
        }

        System.out.println(stringBuilder(term1, term2, rns1, rns2, result,"-"));
    }

    public void mult(long term1, long term2)
    {
        int[] rns1 = convertToRNS(term1);
        int[] rns2 = convertToRNS(term2);
        int[] result = new int[moduli.length];

        for (int i = 0; i < result.length; i++)
        {
            result[i] = (rns1[i]*rns2[i]) % moduli[i];
        }

        System.out.println(stringBuilder(term1, term2, rns1, rns2, result,"*"));
    }

    public void printStats()
    {
        String modulistr = Arrays.toString(moduli);
        DecimalFormat df = new DecimalFormat("0");

        String output = "";
        output += "Low cost moduli: ("+modulistr.substring(1,modulistr.length()-1)+"), ";
        output += "Max column width = " +maxWidth+" bits, ";
        output += "Range = ["+-total/2+", "+(total/2-1)+"], ";
        output += "Efficiency = "+df.format(efficiency)+"%\n";

        System.out.println(output);
    }
}

/*
 * HARDEST PART
 * Getting the optimal moduli for using low cost moduli
 * Ended up using the moduli formula given in the example.
 * Even though it isn't efficient, it is fast.
 *
 *
 * Doing operations:
 * Get the plain text: 180+210
 * Parse into three strings: "180" "+" "210"
 * Send to RNS depending on the operators: +,-,*
 * RNS.add(int 180,int 210)
 *     Convert into residues: convert(180), convert(210)
 *     perform modulus addition into new array
 *     print everything
 *
 *     Use Chinese Remainder Theorem to convert to Dec
 *     print both.
 */
