import java.io.*;
import java.util.*;

/*	Authors: Christopher Knoll, Huidong Hwang
 *	Project name: Perceptron
 *	Date: 03/03/16
 */

public class PerceptronProject
{
	public static void main(String[] args) throws Exception
	{
		if (args.length != 4){
			System.out.println("Usage: PerceptronProject train.dat test.dat alpha iteration");
			return;
		}
		/*	args[0] = Training set
		 *  args[1] = Test set
		 *  args[2] = Learning rate or alpha
		 *  args[3] = Number of iterations
		 */
		
		/*
		 *	Parsing the two datasets
		 */
				
		double learnRate = Double.parseDouble(args[2]);
		int iteration = Integer.parseInt(args[3]);
		
		//Loads in data from the training data
		Scanner cin = new Scanner(new File(args[0]));
		String[] titles = cin.nextLine().split("\t");
		
		//Dataset is an ArrayList of Double arrays where the Double arrays are the instances
		ArrayList<Double[]> dataset1 = new ArrayList<Double[]>();
		while(cin.hasNextLine())
		{
			Double[] intstream = new Double[titles.length];
			String[] datastream = cin.nextLine().split("\t");
			for(int i = 0; i<datastream.length; i++)
				intstream[i] = Double.parseDouble(datastream[i]);
			dataset1.add(intstream);
		}
		
		//Loads in data from the test data
		cin = new Scanner(new File(args[1]));
		
		ArrayList<Double[]> dataset2= new ArrayList<Double[]>();
		//Ignores the titles in the test data
		cin.nextLine();
		while(cin.hasNextLine())
		{
			Double[] intstream = new Double[titles.length];
			String[] datastream = cin.nextLine().split("\t");
			for(int i = 0; i<datastream.length; i++)
				intstream[i] = Double.parseDouble(datastream[i]);
			dataset2.add(intstream);
		}
		
		/*
		 *	Feeding the data into the perceptron
		 */
		
		Perceptron ptn = new Perceptron(dataset1, dataset2, titles, learnRate,iteration);
		ptn.printAccuracy();
		
		cin.close();
	}
}

class Perceptron
{
	private ArrayList<Double[]> trainset, testset;
	private String[] trainTitles;
	private double alpha;
	private int iteration;
	
	public Perceptron(ArrayList<Double[]> dataset1, ArrayList<Double[]> dataset2, String[] titles, double learnRate, int iter)
	{
		trainset = dataset1;
		testset = dataset2;
		trainTitles	= titles;
		alpha = learnRate;
		iteration = iter;
	}
	
	/*
	 *	sigmoid and the sigmoid derivatives
	 *	derivative of sigmoid = sigmoid(1 - sigmoid)
	 */
	
	private double sigmoid (double d)
	{return 1 / (1 + Math.exp(-1 * d));}
	
	private double sigDeriv (double d)
	{return sigmoid(d) - Math.pow(sigmoid(d), 2);}
	
	/*
	 *	Uses the update rules we learned in class
	 */
	
	private double update (double weight, double[] w, Double[] xk, double yk, int i)
	{
		double val = 0.0;
		for(int a = 0; a<w.length; a++)
		{
			val += w[a]*xk[a];
		}
		return weight + (alpha * (yk - sigmoid(val)) * xk[i] * sigDeriv(val));
	}
	
	private double[] train()
	{
		//the weights from the previous iteration
		double[] weights = new double[trainset.get(0).length - 1];
		//the weights from the current iteration
		double[] weightsTemp = new double[trainset.get(0).length - 1];
		int currentIter = 0;
		
		while(iteration > currentIter)
		{
			for(Double[] instance : trainset)
			{
				//Size of weights = the size of the attribute values of the instance
				for(int i = 0; i<weights.length; i++)
				{
					weightsTemp[i] = update(weights[i], weights, instance, instance[weights.length], i);
				}
				//updates the current iteration
				System.arraycopy(weightsTemp, 0, weights, 0, weightsTemp.length);
				
				currentIter++;
				
				//Prints the results after each iteration
				print(weights, currentIter, instance);
				
				//breaks the loop if the iteration size isn't divisible by the dataset size
				if(currentIter >= iteration)
					return weights;
			}
		}
		return weights;
	}
	
	private double accuracy(double[] weights, ArrayList<Double[]> data)
	{
		int accuracy = 0;
		for(Double[] instance : data)
		{
			double val = 0.0;
			//Size of weights = the size of the attribute values of the instance
			for(int a = 0; a<weights.length; a++)
			{
				val += weights[a]*instance[a];
			}
			//The checks the sigmoid of the dot product to classify 
			if((sigmoid(val) >= .5 ? 1 : 0) == instance[instance.length-1])
				accuracy++;
		}
		return (double)accuracy / (double)data.size();
	}

	private void print(double[] weight, int iter, Double[] args)
	{
		System.out.print("After iteration " + iter + ": ");
		double val = 0.0;
		for(int a = 0; a<weight.length; a++)
		{
			val += weight[a]*args[a];
		}
		int count = 0;
		for(double d : weight)
		{
			System.out.printf("w(%s) = %.4f, ", trainTitles[count], d);
			count++;
		}
		System.out.printf("output = %.4f\n", sigmoid(val));
	}
	
	public void printAccuracy()
	{
		double[] weights = train();
		double trainacc = accuracy(weights, trainset);
		double testacc = accuracy(weights, testset);
		System.out.printf("\nAccuracy on training set (%d instances): %3.1f%%\n", trainset.size(), trainacc*100);
		System.out.printf("\nAccuracy on test set (%d instances): %3.1f%%\n", testset.size(), testacc*100);
	}
}