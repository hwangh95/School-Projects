import java.util.*;
import java.io.*;

public class DecisionTree 
{
	public static void main(String[] args) throws Exception
	{
		//load in dataset from file.
		Scanner cin = new Scanner(new File(args[0]));
		String[] titles = cin.nextLine().split("\t");
		ArrayList<Integer[]> dataset = new ArrayList<Integer[]>();
		while(cin.hasNextLine())
		{
			Integer[] intstream = new Integer[titles.length];
			String[] datastream = cin.nextLine().split("\t");
			for(int i = 0; i<datastream.length; i++)
				intstream[i] = Integer.parseInt(datastream[i]);
			dataset.add(intstream);
		}
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
		//prepare a node with the coded dataset as the argument.
		Node head = new Node(dataset);
		//generate tree
		head.split(head);
		//print tree
		head.print(head,  titles);
		//final output lines
		System.out.printf("\nAccuracy on training set (%d instances): %3.1f%%", dataset.size(), 100.0*head.accuracy(dataset));
		System.out.printf("\n\nAccuracy on test set (%d instances): %3.1f%%\n\n", testset.size(), 100.0*head.accuracy(testset));
		cin.close();
	}
}


class Node 
{
	public ArrayList<Integer[]> data;
	public double entropy;
	
	public Node parent;
	public Node[] children = {null, null};
	public int[] splitsOn;
	public int splitAtt;
	boolean visited = false;
	int isLeaf = -1;  // if leaf, has leaf value (0,1)
	static int majority;
	
	//this will generate a node with it's entropy value for you.
	public Node(ArrayList<Integer[]> nodeData, Node parentNode, int[] splits)
	{
		data = nodeData;
		parent = parentNode;
		splitsOn = splits;
		entropy = getEntropy(nodeData);
	}
	
	//used specifically for the head
	public Node(ArrayList<Integer[]> nodeData)
	{
		data = nodeData;
		parent = null;
		splitsOn = new int[nodeData.get(0).length-1];
		Arrays.fill(splitsOn,  -1);
		entropy = getEntropy(nodeData);
		int[] count = {0,0};
		for(int i = 0; i<data.size(); i++)
		{
			int classVal = data.get(i)[data.get(i).length-1];
			if(classVal == 0)
				count[0]++;
			else count[1]++;
		}
		majority = Math.max(count[0],  count[1]) == count[0] ? 0 : 1;
	}
	
	// double for accuracy on set
	public double accuracy(ArrayList<Integer[]> data)
	{
		double correct = 0.0;
		for(Integer[] testVal : data)
		{
			Node check = this;
			while(check.isLeaf == -1)
			{
				if(testVal[check.splitAtt] == 0)
					check = check.children[0];
				else
					check = check.children[1];
			}
			if(check.isLeaf == testVal[testVal.length - 1])
				correct++;
		}
		return correct/data.size();
	}
	
	//calculated weighted entropy based on the entropy of two nodes.
	public double weightedEntropy(ArrayList<Integer[]> a, ArrayList<Integer[]> b)
	{
		double total = a.size() + b.size();
		return (((double)a.size()/total) * getEntropy(a)) + (((double)b.size()/total) * getEntropy(b));
	}
	
	//gets entropy for a single dataset(node to-be)
	public double getEntropy(ArrayList<Integer[]> nodeData)
	{
		int[] count = {0,0};
		for(int i = 0; i<nodeData.size(); i++)
		{
			int classVal = nodeData.get(i)[nodeData.get(i).length-1];
			if(classVal == 0)
				count[0]++;
			else count[1]++;
		}
		int total = count[0]+count[1];
		double fractionA = (double)count[0] / (double)total;
		double fractionB = (double)count[1] / (double)total;
		double ent0 = (count[0] == 0 ? 0 : -1 * fractionA * (Math.log10(fractionA) / Math.log10(2)));
		double ent1 = (count[1] == 0 ? 0 : -1 * fractionB * (Math.log10(fractionB) / Math.log10(2)));
		return ent0 + ent1;
	}
	
	//run this on the head node. It should recurse down the tree to complete the tree.
	public void split(Node n)
	{
		
		ArrayList<Integer[]> testForHomo = new ArrayList<Integer[]>();
		for(Integer[] w : n.data)
		{
			Integer[] add = new Integer[w.length-1];
			for(int x = 0; x<w.length-1; x++)
			{
				add[x] = w[x];
			}
			testForHomo.add(add);
		}
		boolean homogeneous = true;
		for(int y = 0; y<testForHomo.size()-1; y++)
		{
			if(!Arrays.equals(testForHomo.get(y), testForHomo.get(y+1)))
				homogeneous = false;
		}
		if(homogeneous)
		{
			int[] count = {0,0};
			for(int i = 0; i<n.data.size(); i++)
			{
				int classVal = n.data.get(i)[n.data.get(i).length-1];
				if(classVal == 0)
					count[0]++;
				else count[1]++;
			}
			if(count[0] == count[1])
				n.isLeaf = majority;
			else
				n.isLeaf = Math.max(count[0],  count[1]) == count[0] ? 0 : 1;
			return;
		}
		ArrayList<Integer> splitHolder = new ArrayList<Integer>();
		ArrayList<ArrayList<Integer[]>> datasets = new ArrayList<ArrayList<Integer[]>>();
		int splitIndex = 0;
		double[] split = {-1, Double.MAX_VALUE};
		
		for(int i = 0; i<n.splitsOn.length; i++)
			if(n.splitsOn[i] == -1)
			{
				splitHolder.add(i);
				for(int j = 0; j<2; j++)
				{
					ArrayList<Integer[]> newSet = new ArrayList<Integer[]>();
					for(Integer[] a : n.data)
						if(a[i] == j)
							newSet.add(a);	
					datasets.add(newSet);
				}
			}
		for(int k = 0; k<datasets.size(); k = k+2)
		{
			if(weightedEntropy(datasets.get(k), datasets.get(k+1)) < split[1])
			{
				split[0] = k;
				split[1] = weightedEntropy(datasets.get(k), datasets.get(k+1));
				splitIndex = k/2;
			}
		}
		int[] left = new int[n.splitsOn.length];
		int[] right = new int[n.splitsOn.length];
		System.arraycopy(n.splitsOn,  0,  left, 0, n.splitsOn.length);
		System.arraycopy(n.splitsOn,  0,  right, 0, n.splitsOn.length);
		if(splitHolder.size() == 0)
			return;
		n.splitAtt = splitHolder.get(splitIndex);
		left[n.splitAtt] = 0;
		right[n.splitAtt] = 1;
		Node childLeft = new Node(datasets.get((int)split[0]), n, left);
		Node childRight = new Node(datasets.get((int)(split[0]+1)), n, right);
		n.children[0] = childLeft;
		n.children[1] = childRight;
		boolean leftCont = false;
		boolean rightCont = false;
		for(int u : left)
			if(u == -1)
				leftCont = true;
		for(int v : right)
			if(v == -1)
				rightCont = true;
		if(n.children[0].entropy != 0.0 && leftCont)
			split(childLeft);
		else
		{
			int[] count = {0,0};
			for(int i = 0; i<n.children[0].data.size(); i++)
			{
				int classVal = n.children[0].data.get(i)[n.children[0].data.get(i).length-1];
				if(classVal == 0)
					count[0]++;
				else count[1]++;
			}
			if(count[0] == count[1])
				n.children[0].isLeaf = majority;
			else
				n.children[0].isLeaf = Math.max(count[0],  count[1]) == count[0] ? 0 : 1;
		}
		if(n.children[1].entropy != 0.0 && rightCont)
			split(childRight);
		else
		{
			int[] count = {0,0};
			for(int i = 0; i<n.children[1].data.size(); i++)
			{
				int classVal = n.children[1].data.get(i)[n.children[1].data.get(i).length-1];
				if(classVal == 0)
					count[0]++;
				else count[1]++;
			}
			if(count[0] == count[1])
				n.children[1].isLeaf = majority;
			else
				n.children[1].isLeaf = Math.max(count[0],  count[1]) == count[0] ? 0 : 1;
		}
	}
	
	//prints the tree
	public void print(Node n, String[] titles)
	{
		//if the parent has two null or visited children, and is visited itself, return
		if(n.parent == null && n.visited && n.children[1].visited)
			return;
		//if the left child is not a leaf node and is not visited, go into it
		if(n.children[0].children[0] != null && !n.children[0].children[0].visited)
		{
			Node check = n;
			while(check.parent != null)
			{
				System.out.print("| ");
				check = check.parent;
			}
			System.out.print(titles[n.splitAtt] + " = 0 :\n");
			print(n.children[0], titles);
		}
		//if the left child is a leaf node, visit it and display it's contents
		else
		{
			Node check = n;
			while(check.parent != null)
			{
				System.out.print("| ");
				check = check.parent;
			}
			System.out.print(titles[n.splitAtt] + " = 0 :  " + n.children[0].isLeaf + "\n");
			n.children[0].visited = true;
		}
		//if the right child is not a leaf node and not visited, go into it
		if(n.children[1].children[0] != null && !n.children[1].children[0].visited)
		{
			Node check = n;
			while(check.parent != null)
			{
				System.out.print("| ");
				check = check.parent;
			}
			System.out.print(titles[n.splitAtt] + " = 1 :\n");
			print(n.children[1], titles);
		}
		// if the right child is a leaf node, visit it and display it's contents
		else
		{
			Node check = n;
			while(check.parent != null)
			{
				System.out.print("| ");
				check = check.parent;
			}
			System.out.print(titles[n.splitAtt] + " = 1 :  " + n.children[1].isLeaf + "\n");
			n.children[1].visited = true;
		}
		//finally, visit the node you're in (this is for moving back up the tree)
		n.visited = true;
	}
}