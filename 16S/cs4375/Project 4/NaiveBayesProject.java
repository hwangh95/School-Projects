import java.util.*;
import java.io.*;

public class NaiveBayesProject
{	
	public static void main(String[] args) throws Exception
	{
		//load in training set from file.
		Scanner cin = new Scanner(new File(args[0]));
		String[] titles = cin.nextLine().split("\t");
		ArrayList<Integer[]> training = new ArrayList<Integer[]>();
		while(cin.hasNextLine())
		{
			Integer[] intstream = new Integer[titles.length];
			String[] datastream = cin.nextLine().split("\t");
			for(int i = 0; i<datastream.length; i++)
				intstream[i] = Integer.parseInt(datastream[i]);
			training.add(intstream);
		}
		NaiveBayes nb = new NaiveBayes(training,titles);
		nb.genValues();
		nb.print();
		
		//load in test set from file
		cin = new Scanner(new File(args[1]));
		ArrayList<Integer[]> testset= new ArrayList<Integer[]>();
		cin.nextLine();
		while(cin.hasNextLine())
		{
			Integer[] intstream = new Integer[titles.length];
			String[] datastream = cin.nextLine().split("\t");
			for(int i = 0; i<datastream.length; i++)
				intstream[i] = Integer.parseInt(datastream[i]);
			testset.add(intstream);
		}
		//final output lines
		System.out.printf("\nAccuracy on training set (%d instances): %3.1f%%", training.size(), 100.0*nb.accuracy(training));
		System.out.printf("\nAccuracy on test set (%d instances): %3.1f%%", testset.size(), 100.0*nb.accuracy(testset));
		cin.close();
	}
}
class NaiveBayes
{
	public ArrayList<Integer[]> dataset;
	public double probC;
	public double[][] probAttr;
	public String[] titles;
	
	public NaiveBayes(ArrayList<Integer[]> ds, String[] values)
	{
		dataset = ds;
		titles = values;
	}
	public void genValues()
	{
		double c = 0;
		for(Integer[] j : dataset)
		{	
			if(j[j.length-1] == 1)
				c++;
		}
		probC = c/dataset.size();
		
		probAttr = new double[2][dataset.get(0).length-1];
		for(int i = 0; i<dataset.get(0).length-1; i++)
		{
			probAttr[0][i] = value(i,0);
			probAttr[1][i] = value(i,1);
		}
	}
	
	public double value(int attr, int is_c)
	{
		double approved = 0;
		int total = 0;
		for(int i = 0; i<dataset.size(); i++)
		{	
			if(dataset.get(i)[dataset.get(i).length-1] == is_c)
			{
				total++;
				if(dataset.get(i)[attr] == 1)
					approved++;
			}
		}
		return approved/total;
	}
	
	public void print()
	{
		for(int i=0; i<2; i++)
		{
			System.out.print("P("+titles[titles.length-1]+"="+i+")=");
			if(i==0)
				System.out.printf("%1.2f",1-probC);
			else System.out.printf("%1.2f",probC);
			System.out.print(" ");
			for(int j = 0; j<titles.length-1; j++)
			{
				System.out.print("P("+titles[j]+"=0|"+i);
				System.out.printf(")=%1.2f", probAttr[i][j]);
				System.out.print(" ");
				System.out.print("P("+titles[j]+"=1|"+i);
				System.out.printf(")=%1.2f", 1-probAttr[i][j]);
				System.out.print(" ");
			}
			System.out.println();
		}
	}
	
	public int classify(Integer[] args)
	{
		double prob = probC;
		for(int i = 0; i<probAttr[1].length; i++)
		{
			if(args[i] == 1)
				prob *= probAttr[1][i];
			else prob *= 1-probAttr[1][i];
		}
		double prob_false = (1.0 - probC);
		for(int j = 0; j<probAttr[0].length; j++)
		{
			if(args[j] == 1)
				prob_false *= probAttr[0][j];
			else prob_false *= 1-probAttr[0][j];
		}
		if(prob>prob_false)
			return 1;
		return 0;
	}
	
	public double accuracy(ArrayList<Integer[]> data)
	{
		int accuracy = 0;
		for(int i = 0; i<data.size(); i++)
		{
			Integer[] args = Arrays.copyOfRange(data.get(i), 0, data.get(i).length-1);
			if(classify(args) == data.get(i)[data.get(i).length-1])
				accuracy++;
		}
		return (double)accuracy / (double)data.size();
	}	
}