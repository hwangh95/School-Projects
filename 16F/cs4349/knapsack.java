// Huidong Hwang
// 10/30/16
// Computes the 0-1 knapsack problem

import java.io.*;
import java.util.*;

/*
Choices: 
303665 307251 295812 296394 287798 288302 279950 288465 271928 272280 264089 266904 256061 256257 248226 248348 240200 249484 232365 232331 225622 224343 221134 226391 218889 216646 216502 216316 209912 207669 208488 205424 208224 203181 200936 200640 197492 204148 194204 193006 191959 192631 189716 192215 187471 186275 185228 184033 184775 181789 186052 179547 178494 177303 176251 176776 174005 176204 170161 171762 169037 170574 169517 168330 168913 166088 168262 163844 162785 160146 161601 160540 160919 156832 158296 155701 164815 156051 154871 152389 153808 151255 152629 150167 153048 149034 150385 147946 152245 145800 146810 148142 145724 147074 144588 145898 143503 144831 142364 145064 141281 142585 140143 145570 139060 140342 137919 139170 136838 138096 135697 136926 134617 137184 133473 134683 132395 136226 131252 132439 130174 131364 129028 130197 127952 129119 126806 129207 125731 126876 123499 124582 128174 123200 122897 123508 124630 121895 121598 121300 122361 123466 120999 120698 120393 121288 122387 119779 119693 119398 119099 120137 121224 118800 118497 118194 118081 119065 121318 117581 117494 117270 117197 116900 117915 118980 116599 116298 116171 115993 115879 116845 124828 115380 115293 115072 114998 114834 114760 114699 115691 116738 114400 114259 114098 113969 113795 113680 114622 115652 113182 113094 112920 112872 112797 112632 112562 112500 113469 114493 112199 112058 111938 111899 111769 111623 111594 111478 111310 112401 113409 111005 110981 110892 110718 110673 110598 110433 110362 110299 111246 113351 110000 109858 109737 109698 109568 109425 109395 109278 109110 110179 111164 108803 108783 108693 108518 108473 108396 108231 108163 108100 109024 112163 107799 107656 107539 107499 107368 107225 107195 107077 106912 107958 108920 106603 106582 106492 106316 106274 106197 106031 105963 105899 106800 107765 105600 105456 105339 105298 105167 105027 104996 104877 104712 105736 106675 104401 104384 104292 104116 104074 103996 103829 103765 103700 104578 105520 103399 103255 103141 103099 102967 102827 102795 102676 102514 103515 105454 102201 102183 102091 101914 101876 101797 101629 101565 101499 102355 103278 101200 101055 100941 100898 100765 100628 100596 100476 100314 101292
Final value:
11528785

*/
public class knapsack
{
    public static void main(String[] args) throws Exception
    {
        Scanner cin = new Scanner(new File("hw3_data"));
        String[] numbers = cin.nextLine().split(" ");
        int n = Integer.parseInt(numbers[0]);
        int w = Integer.parseInt(numbers[1]);

        int[][] data = new int[n][w];
        int[] weights = new int[n];
        int[] vals = new int[n];

        int count = 0;
        while (cin.hasNextLine()){
            String[] entry = cin.nextLine().split(" ");
            weights[count] = Integer.parseInt(entry[1]);
            vals[count++] = Integer.parseInt(entry[2]);
        }
        for (int i = 1; i<n;i++)
        {
            for (int j = 0; j<w;j++)
            {
                if (weights[i]>j)
                {
                    data[i][j] = data[i-1][j];
                }
                else
                {
                    data[i][j] = Math.max(data[i-1][j],data[i-1][j-weights[i]]+vals[i]);
                }
            }
        }
        String choices = "";
        for (int i = n-1; i >0; i--)
        {
            if (data[i][w-1] - data[i-1][w-1-weights[i]] == vals[i])
            {
                choices += vals[i] + " ";
            }
        }
        System.out.println(choices+"\n"+data[n-1][w-1]);
        
    }
}
