#!/usr/bin/python

"""
**********************************************************************************************************************************************************
*Authors         :       Amar Bhagwandas Lalwani (MT2012073) and Raghav Bali (MT2012108)
*
*Date            :       May 18 2013
*
*Project         :       Hierarchical Clustering System (HCS)
*Version         :       1.0
*Description     :       This is a demo to showcase the working of "Hierarchical Clustering Algorithm" based on globa;l and cluster frequent-itemsets.
*                        This demo works on a sample corpus which is preprocessed by "The Boolean Retrieval System (TBRS)" developed by the same authors.
*                        TBRS is used to generate "Term-Document-Frequency" triplet as output which is used to generate Hierarchical Clusters.
*
*Dependencies    :       The Boolean Retrieval System (TBRS) for preprocessing documents for clustering. [Developed by same authors]
*                        "python-fp-growth" for finding frequent itemsets. [Developed by Eric Naeseth]
**********************************************************************************************************************************************************
"""


"""
Import Packages
"""
import clustering_v1 ;


"""
Global Constants Defined
"""
# Global Minimum Support value. It should be ideally around 4-10% of the total documents.
global_minsup=4;

# Cluster Minimum Support value. 0.25 is a good value.
cluster_minsup=0.25;


"""
Function        :       generate_input_for_fpgrowth_and_document_vectors
Description     :       Generate Global Frequent Itemsets.Generate Transactions for Hierachical Clustering
Input Params    :       NA
Output          :       A list of the form : List[0] is a list of transactions (terms/document)
                                             List[1] is a dictionary of the form {docname,transaction vector}
"""
output=clustering_v1.generate_input_for_fpgrowth_and_document_vectors();


"""
Function        :       find_global_freq_itemsets
Description     :       Generate Global Frequent Itemsets.
Input Params    :       Parameter 1 : List of unique terms/document, transaction vector.
                        Parameter 2 : Global minimum support value.
Output          :       A list of global frequent itemsets
"""
itemsets=clustering_v1.find_global_freq_itemsets(output[0],global_minsup);



"""
Function        :       find_feature_vectors_from_doc_vectors
Description     :       Generate Feature Vectors for each of the documents. Dimensionality reduction step
Input Params    :       Parameter 1 : List of global frequent itemsets
                        Parameter 2 : Dictionary of the form {docname,transaction vector}
Output          :       A dictionary of the form {docname,transaction vector} with transaction vector having terms with non-zero freqencies
"""
feature_vectors=clustering_v1.find_feature_vectors_from_doc_vectors(itemsets, output[1]);


"""
Function        :       find_initial_assignment
Description     :       Assign Documents to intial clusters. Documents may be assigned to multiple clusters at this moment.
Input Params    :       Parameter 1 : List of global frequent itemsets
                        Parameter 2 : Dictionary of the form {docname,transaction vector} with transaction vector having terms with non-zero freqencies
Output          :       A list of dictionaries. List[0] is a dictionary of the form {frequent itemsets, documents assigned}
                                                List[1] is a dictonary of the form {documents, frequent itemssets}
"""
assgnmnts=clustering_v1.find_initial_assignment(itemsets, feature_vectors);


"""
Function        :       find_disjoint_clusters
Description     :       Assign Documents to disjoint clusters. Documents are assigned only to one cluster at this step.
Input Params    :       Parameter 1 : Document Cluster. Dictonary of the form {documents, frequent itemssets}
                        Parameter 2 : Initial Assignment. Dictionary of the form {frequent itemsets, documents assigned}
                        Parameter 3 : Feature Vectors. A dictionary of the form {docname,transaction vector} with transaction vector
                                      having terms with non-zero freqencies
                        Parameter 4 : A list of global frequent itemsets
                        Parameter 5 : Cluser Minimum Support Values
Output          :       A dictionary of the form {frequent itemsets, document vectors} 
"""
clustering=clustering_v1.find_disjoint_clusters(assgnmnts[1],assgnmnts[0], feature_vectors, itemsets, cluster_minsup);


"""
Printing the disjoint clusters formed above in clustering.txt
"""
fp=open('../ClusterOutput/disjoint_clusters.txt','w');
cluster_tuples=clustering.items();
pruned_tuples = {}
for cluster in cluster_tuples:
	if(len(cluster[1])>0):
                #Print the cluster label
		fp.write(str(cluster[0]));
		fp.write('-');
		pruned_tuples[cluster[0]]=cluster[1];
		for docs in cluster[1]:
			if(docs==cluster[1][len(cluster[1])-1]):
                                #Print the document name
				fp.write(docs);
				fp.write('\n');
			else:
                                #Print the document name
				fp.write(docs);
				fp.write(',');
fp.close();
print "[*]Disjoint clusters : disjoint_clusters.txt [done] "

"""
Function        :       find_descendants
Description     :       Find descendants for all disjoint clusters to find cluster frequnet items and cluster support in score calculation.
Input Params    :       Parameter 1 : Disjoint Document Clusters. A dictionary of the form {frequent itemsets, document vectors} 
                        Parameter 2 : Disjoint Document Clusters with non-empty document lists. Dictionary of the form {frequent itemsets, documents assigned}
                        Parameter 3 : A list of global frequent itemsets
Output          :       A dictionary of the form {frequent itemsets, document vectors}
"""
descendants=clustering_v1.find_descendants(clustering, pruned_tuples, itemsets);

"""
Print the descendants of each of the disjoint clusters
"""
fp=open('../ClusterOutput/descendants.txt','w');
cluster_tuples=descendants.items();
for cluster in cluster_tuples:
	if(len(cluster[1])>0):	
		fp.write(str(cluster[0]));
		fp.write('-');
		
		for docs in cluster[1]:
			if(docs==cluster[1][len(cluster[1])-1]):
				fp.write(docs);
				fp.write('\n');
			else:
				fp.write(docs);
				fp.write(',');
fp.close();
print "[*]Descendants : descendants.txt [done] "

"""
Function        :       delete_empty_leaf_nodes
Description     :       Delete all empty leaf clusters
Input Params    :       Parameter 1 : Disjoint Document Clusters. A dictionary of the form {frequent itemsets, document vectors} 
                        Parameter 2 : A list of global frequent itemsets
Output          :       A dictionary of the form {frequent itemsets, document vectors} with non-empty leaf nodes
"""
refined_clusters=clustering_v1.delete_empty_leaf_nodes(clustering, itemsets);



"""
Function        :       build_tree
Description     :       Build Tree structure from disjoint clusters pruned in previous steps
Input Params    :       Parameter 1 : A list of global frequent itemsets
                        Parameter 2 : Feature Vectors. A dictionary of the form {docname,transaction vector} with transaction vector
                                      having terms with non-zero freqencies
                        Parameter 3 : Descendants.  A dictionary of the form {frequent itemsets, document vectors}
                        Parameter 4 : A dictionary of the form {frequent itemsets, document vectors} with non-empty leaf nodes
                        Parameter 5 : Cluster Minimum Support value
Output          :       A list of dictionaries. 
"""
tree=clustering_v1.build_tree(itemsets, feature_vectors, descendants, refined_clusters, cluster_minsup);


"""
Print the Tree structure
"""
fp=open('../ClusterOutput/initial_cluster_tree.txt','w');
cluster_tuples=tree[0].items();
for cluster in cluster_tuples:
	if(len(cluster[1])>0):	
		fp.write(str(cluster[0]));
		fp.write('-');
		
		for docs in cluster[1]:
			if(docs==cluster[1][len(cluster[1])-1]):
				fp.write(docs);
				fp.write('\n');
			else:
				fp.write(docs);
				fp.write(',');
fp.close();
print "[*]Initial Tree : initial_cluster_tree.txt [done] "

"""
Function        :       child_prune
Description     :       Prune Child nodes in a chain of clusters
Input Params    :       Parameter 1 : A dictionary of the form {frequent itemsets, document vectors} with non-empty leaf nodes
                        Parameter 2 : Tree[0]
                        Parameter 3 : Tree[1]
                        Parameter 4 : A list of global frequent itemsets
                        Parameter 5 : Feature Vectors. A dictionary of the form {docname,transaction vector} with transaction vector
                                      having terms with non-zero freqencies
                        Parameter 6 : A dictionary of the form {frequent itemsets, document vectors}
                        Parameter 7 : Cluster Minimum support values 
Output          :       Prune d tree dictionary
"""
pruning=clustering_v1.child_prune(refined_clusters, tree[0], tree[1], itemsets, feature_vectors, descendants, cluster_minsup);


"""
Print Pruned clusters
"""
fp=open('../ClusterOutput/pruned_clusters.txt','w');
cluster_tuples=pruning[0].items();
for cluster in cluster_tuples:
	if(len(cluster[1])>0):	
		fp.write(str(cluster[0]));
		fp.write('-');
		
		for docs in cluster[1]:
			if(docs==cluster[1][len(cluster[1])-1]):
				fp.write(docs);
				fp.write('\n');
			else:
				fp.write(docs);
				fp.write(',');
fp.close();
print "[*]Pruned Clusters : pruned_clusters.txt [done] "

"""
Print Pruned Clusters
"""
fp=open('../ClusterOutput/pruned_tree.txt','w');
cluster_tuples=pruning[1].items();
for cluster in cluster_tuples:
	if(len(cluster[1])>0):	
		fp.write(str(cluster[0]));
		fp.write('-');
		
		for docs in cluster[1]:
			if(docs==cluster[1][len(cluster[1])-1]):
				fp.write(docs);
				fp.write('\n');
			else:
				fp.write(docs);
				fp.write(',');
fp.close();
print "[*]Pruned Tree : pruned_tree.txt [done] "



"""
Function        :       merge_siblings
Description     :       Prune Child nodes in a chain of clusters
Input Params    :       Parameter 1 : Pruned Tree [0]
                        Parameter 2 : Pruned Tree[1]
                        Parameter 3 : Pruned Tree[2]
                        Parameter 4 : A list of global frequent itemsets
                        Parameter 5 : Feature Vectors. A dictionary of the form {docname,transaction vector} with transaction vector
                                      having terms with non-zero freqencies
                        Parameter 6 : A dictionary of the form {frequent itemsets, document vectors}
                        Parameter 7 : Cluster Minimum support values 
Output          :       Sibling merged tree dictionary
"""
final=clustering_v1.merge_siblings(pruning[0], pruning[1], pruning[2], itemsets, feature_vectors, descendants, cluster_minsup);


"""
Print the final output
"""
fp=open('../ClusterOutput/output_clusters.txt','w');
cluster_tuples=final[0].items();
for cluster in cluster_tuples:
	if(len(cluster[1])>0):	
		fp.write(str(cluster[0]));
		fp.write('-');
		fp.write(str(cluster[1]));
		fp.write('\n');
fp.close();
print "[*]Output Clusters : output_clusters.txt [done] "

"""
Print the parents
"""
fp=open('../ClusterOutput/output_tree.txt','w');
cluster_tuples=final[1].items();
for cluster in cluster_tuples:
	if(len(cluster[1])>0):	
		fp.write(str(cluster[0]));
		fp.write('-');
		fp.write(str(cluster[1]));
		fp.write('\n');
fp.close();
print "[*]Output Tree : output_tree.txt [done] "


"""
Function        :       find_cluster_description
Description     :       Describe the clusters labels with corresponding cluster frequent items
Input Params    :       Parameter 1 : Final Tree [0]
                        Parameter 2 : Final Tree [1]
                        Parameter 3 : Final Tree [2]
                        Parameter 4 : Feature Vectors. A dictionary of the form {docname,transaction vector} with transaction vector
                                      having terms with non-zero freqencies
                        Parameter 5 : list of global frequent itemsets
                        Parameter 6 : Cluster Minimum support values 
Output          :       Dictionary of cluster label description
"""
description=clustering_v1.find_cluster_description(final[0], final[1], final[2], feature_vectors, itemsets, cluster_minsup);

"""
Print the cluster description
"""
fp=open('../ClusterOutput/cluster_description.txt','w');
cluster_tuples=description.items();
for cluster in cluster_tuples:
        if(len(cluster[1])>0):
                fp.write(str(cluster[0]));
                fp.write('-');
                fp.write(str(cluster[1]));
                fp.write('\n');
fp.close();
print "[*]Cluster Description : cluster_description.txt [done] "


"""
**********************************************************************************************************************************************************
                                                                        End of Code
**********************************************************************************************************************************************************                                        
"""

