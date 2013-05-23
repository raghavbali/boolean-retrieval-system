#!/usr/bin/python


"""
**********************************************************************************************************************************************************
*Authors         :       Amar Bhagwandas Lalwani (MT2012073) and Raghav Bali (MT2012108)
*
*Date            :       May 18 2013
*
*Project         :       Hierarchical Clustering System (HCS)
*Version         :       1.0
*Description     :       This is the implementation of the Hierarchichal Clustering algorithm. Each of the methods have been provided with
*                        detailed explaination along with information regarding function signatures etc.
*                        For details regarding dependencies, please look into corresponding project repositories.
*
*Dependencies    :       The Boolean Retrieval System (TBRS) for preprocessing documents for clustering. [Developed by same authors]
*                        "python-fp-growth" for finding frequent itemsets. [Developed by Eric Naeseth]
**********************************************************************************************************************************************************
"""


"""
Import Packages
"""
import math
from fp_growth import find_frequent_itemsets;
import itertools;

"""
Description : 
"input.txt" has the names of the documents in separate lines. "postingslist.txt" has the output of the postings list with term,
document and its frequency. This function returns a list containing 'transactions' which will serve as the input for frequent itemset generation.
It is a list of lists where each list is set of unique terms in a document.
The second element of the output list is the dictionary of document vectors where keys are the names of the documents and
the values are the unique terms present in it.
"""
"""
Function        :       generate_input_for_fpgrowth_and_document_vectors
Description     :       Generate Global Frequent Itemsets.Generate Transactions for Hierachical Clustering
Input Params    :       NA
Output          :       A list of the form : List[0] is a list of transactions (terms/document)
                                             List[1] is a dictionary of the form {docname,transaction vector}
"""
def generate_input_for_fpgrowth_and_document_vectors():
	fp=open('input.txt','r');
	documents=fp.read().splitlines();
	fp.close();
	fp=open('postingslist.txt','r');
	pairs=fp.read().splitlines();
	fp.close();
	transactions=[];
	doc_vectors={};
	output=[];
	for document in documents:
		vector=[];
		copy_pairs=pairs;
		#print copy_pairs;
		for pair in copy_pairs:
			list_of_tuple=pair.split(' ');
			term=list_of_tuple[0];
			doc=list_of_tuple[1];
			if(document==doc):
				vector.append(term);
		transactions.append(vector);
		doc_vectors[document]=vector;
	output.append(transactions);
	output.append(doc_vectors);
	return output;		



"""
Description :
This function takes the dictionary of document vectors as the input and persists it in the file name given as the input
"""
"""
Function        :       persist_doc_vectors
Description     :       Deprecated Function
"""
def persist_doc_vectors(doc_vectors,filename):
	fp=open(filename,'w');
	pairs=doc_vectors.items();
	docs=len(doc_vectors);
	for pair in pairs:
		fp.write(pair[0]+'-');
		for terms in pair[1]:
			count_of_terms=len(pair[1]);
			if(terms==pair[1][count_of_terms-1]):
				fp.write(terms);
			else:
				fp.write(terms+',');
		if(pair!=pairs[docs-1]):
			fp.write('\n');
	fp.close();

"""
Description :
This function generates the global frequent itemsets and outputs it in the form of the list of those sets.
"""
"""
Function        :       find_global_freq_itemsets
Description     :       Generate Global Frequent Itemsets.
Input Params    :       Parameter 1 : List of unique terms/document, transaction vector.
                        Parameter 2 : Global minimum support value.
Output          :       A list of global frequent itemsets
"""
def find_global_freq_itemsets(transactions,minsup):
	global_freq_itemset=[];
	for itemset in find_frequent_itemsets(transactions, minsup):
		global_freq_itemset.append(itemset); 
	return global_freq_itemset;


"""
Description :
This function takes list of global frequent itemsets and the filename in which the document vectors were persisted and generates the feature vectors.
It outputs the dictionary with document names as the keys and the unique features as the values in the form of list for each document.
Feature vectors have too less terms that the document vectors (reduced dimensions).
"""
"""
Function        :       find_feature_vectors
Description     :       Deprecated Function
"""
def find_feature_vectors(freq_itemsets, filename):
	freq_items=[];
	for sets in freq_itemsets:
		for terms in sets:
			if(freq_items.count(terms)==0):
				freq_items.append(terms);	
	fp=open(filename,'r');
	doc_vectors=fp.read().splitlines();
	fp.close();
	feature_vectors={};
	for vectors in doc_vectors:
		storage=vectors.split('-');
		doc=storage[0];
		list_of_terms=storage[1].split(',');
		temp_list=[];
		for terms in list_of_terms:
			if(freq_items.count(terms)!=0):
				temp_list.append(terms);
		feature_vectors[doc]=temp_list;
	return feature_vectors;




"""
Description :
This function takes list of global frequent itemsets and the filename in which the document vectors were persisted and generates the feature vectors.
It outputs the dictionary with document names as the keys and the unique features as the values in the form of list for each document.
Feature vectors have too less terms than the document vectors (reduced dimensions).
"""
"""
Function        :       find_feature_vectors_from_doc_vectors
Description     :       Generate Feature Vectors for each of the documents. Dimensionality reduction step
Input Params    :       Parameter 1 : List of global frequent itemsets
                        Parameter 2 : Dictionary of the form {docname,transaction vector}
Output          :       A dictionary of the form {docname,transaction vector} with transaction vector having terms with non-zero freqencies
"""
def find_feature_vectors_from_doc_vectors(freq_itemsets, document_vectors):
	freq_items=[];
	for sets in freq_itemsets:
		for terms in sets:
			if(freq_items.count(terms)==0):
				freq_items.append(terms);	
	
	documents=document_vectors.keys();
	for document in documents:
		for terms in document_vectors[document]:
			if(freq_items.count(terms)==0):
				document_vectors[document].remove(terms);
	return document_vectors






"""
Description :
This function takes the list of global frequent itemsets as the input in the form of list and the dictionary of feature vectors.
It outputs a list contaning two elements. First is the dictionary where keys are the tuples of frequent itemsets and the values
are the list of the document names in that cluster. So each entry or each tuple represents a cluster.
The second dictionary has the document names as the keys and the list of the itemsets (clusters) to which it belongs to.
In this initial assignment, a document can belong to multiple clusters
"""
"""
Function        :       find_initial_assignment
Description     :       Assign Documents to intial clusters. Documents may be assigned to multiple clusters at this moment.
Input Params    :       Parameter 1 : List of global frequent itemsets
                        Parameter 2 : Dictionary of the form {docname,transaction vector} with transaction vector having terms with non-zero freqencies
Output          :       A list of dictionaries. List[0] is a dictionary of the form {frequent itemsets, documents assigned}
                                                List[1] is a dictonary of the form {documents, frequent itemssets}
"""
def find_initial_assignment(freq_itemsets,feature_vectors):
	output=[];
	initial_assignment={};
	doc_assignment={};
	for itemsets in freq_itemsets:
		initial_assignment[tuple(itemsets)]=[];
	doc_pairs=feature_vectors.items();
	for doc_pair in doc_pairs:
		doc_assignment[doc_pair[0]]=[];
		for itemsets in freq_itemsets:
			flag=1;
			for terms in itemsets:
				if(doc_pair[1].count(terms)==0):
					flag=0;
					break;
			if(flag==1):
				initial_assignment[tuple(itemsets)].append(doc_pair[0]);
				doc_assignment[doc_pair[0]].append(itemsets);
	output.append(initial_assignment);
	output.append(doc_assignment);
	return output;

"""
Description :
This function is used to find the cluster frequent items and their cluster support for all clusters just after initial cluster assignment.
It takes the dictionary of initial assignment, dictionary of feature vectors, list of global frequent itemsets and minimum support in percentage as input.
It outputs the dictionary with the tuple of the cluster lables as the keys and the list of the cluster frequent items of that cluster as the values.
Also it writes the cluster support of the cluster frequent items in perccentage in the file "initial_cluster_frequent_items.txt".
"""
"""
Function        :       find_cluster_frequent
Description     :       Internal Function
"""
def find_cluster_frequent(clusters, feature_vectors, freq_itemsets, minsup):
	freq_items=[];
	for sets in freq_itemsets:
		for terms in sets:
			if(freq_items.count(terms)==0):
				freq_items.append(terms);		
	fp=open('initial_cluster_frequent_items.txt','w');
	cluster_freq={};
	cluster_tuples=clusters.items();
	for cluster in cluster_tuples:
		list_of_freq_terms=[];
		no_of_documents=len(cluster[1]);
		for terms in freq_items:
			count=0;
			for docs in cluster[1]:
				if(feature_vectors[docs].count(terms)>0):
					count=count+1;
			support=count/(no_of_documents+1);
			if((support*100)>=minsup):
				list_of_freq_terms.append(terms);
				for items in cluster[0]:
					if(items!=cluster[0][len(cluster[0])-1]):
						fp.write(items+',');
					else:
						fp.write(items+'-'+terms+' '+str((support*100))+'\n');
		cluster_freq[cluster[0]]=list_of_freq_terms;
	fp.close();	
	return cluster_freq;


"""
Description :
This function takes the term, list of the items, that is, the itemset representing the cluster and the dictionary of the cluster_freq items.
It outputs "1" if the term is cluster frequent in that cluster and otherwise "0".
"""
def is_cluster_freq(term, cluster, cluster_freq):
	if(cluster_freq[tuple(cluster)].count(term)>0):
		return 1;
	else:
		return 0;

"""
Description :
This function takes the term, the global frequent itemsets (their list) and the dictionary of feature vectors as input and returns the global support of that term.
"""
"""
Function        :       find_global_support
Description     :       Internal Function
"""
def find_global_support(term, freq_itemsets, feature_vectors):
	freq_items=[];
	for sets in freq_itemsets:
		for terms in sets:
			if(freq_items.count(terms)==0):
				freq_items.append(terms);		
	no_of_docs=len(feature_vectors);
	transactions=feature_vectors.values();
	count=0;
	for vectors in transactions:
		if(vectors.count(term)>0):
			count=count+1;
	support=(float(count)/float(no_of_docs+1));
	return float(support);


"""
Description :
This function takes the term, list of the items of the cluster (cluster label),
dictionary of the initial assignment of docs and the dictionary of the feature vectors of the docs.
It outputs the cluster support of that term in that cluster.
"""
"""
Function        :       find_cluster_support
Description     :       Internal Function
"""
def find_cluster_support(term, cluster, initial_assgnmnt, feature_vectors):
        if initial_assgnmnt.has_key(tuple(cluster)):
                no_of_docs=len(initial_assgnmnt[tuple(cluster)]);
        else:
                return 0;
	count=0;
	for docs in initial_assgnmnt[tuple(cluster)]:
		if(feature_vectors.has_key(docs) and feature_vectors[docs].count(term)>0):
			count=count+1;
	support=(float(count)/float(no_of_docs+1));
	return float(support);


"""
Description :
This function find the tf-idf for the given term of the given document.
"""
"""
Function        :       find_tf_idf
Description     :       Internal Function
"""
def find_tf_idf(term, document):
	fp=open('input.txt','r');
	no_of_docs=len(fp.read().splitlines());
	fp.close();
	fp=open('postingslist.txt','r');
	entries=fp.read().splitlines();
	fp.close();
	tf=0;
	count=0;
	for entry in entries:
		vector=entry.split(' ');
		word=vector[0];
		doc=vector[1];
		freq=vector[2];
		if(term == word):
			count=count+1;
			if(document==doc):
				tf=float(freq);
	idf=math.log(float(no_of_docs)/(float(count)+1));
	return float(tf*idf);


"""
Description :
This function takes the document name, the list of terms representing the cluster, dictionary of feature vectors, dictionary of initial assignment,
global frequent itemsets and minimim cluster support as input. The output is score of that document against that cluster.
"""
"""
Function        :       get_score_new
Description     :       Internal Function
"""
def get_score_new(document, cluster, feature_vectors, initial_assgnmnt, freq_itemsets, minsup):
	score=0;
	support=0;
	for terms in feature_vectors[document]:
                
		support=find_cluster_support(terms, cluster, initial_assgnmnt, feature_vectors);
		if(support<minsup):
			support=(-1)*find_global_support(terms, freq_itemsets, feature_vectors);	
		score=score+support*find_tf_idf(terms, document);
	return score;


"""
Description :
This function takes the dictionary of the initial assignment subject to the documents, dictionary of the initial assignment subject to clusters,
dictionary of feature vectors, global frequent itemsets and the minimim cluster support.
It gives a dictionary as output which is the assignment of the documents to the clusters.
This dictionary is disjoint, that is, each document belongs to just one cluster.
The keys are the tuples of cluster labels and values are the list of documemts in that cluster.
"""
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
def find_disjoint_clusters(doc_clusters, initial_assgnmnt, feature_vectors, freq_itemsets,min_cluster_sup):
	disjoint_assgnmnt={};
	cluster_labels=initial_assgnmnt.items();
	for cluster_label in cluster_labels:
		disjoint_assgnmnt[cluster_label[0]]=[];
	doc_assgnmnts=doc_clusters.items();
	for doc_vectors in doc_assgnmnts:
		optimum=-100;
		opt_list=[];
		for clusters in doc_vectors[1]:
			temp_score=get_score_new(doc_vectors[0],clusters, feature_vectors, initial_assgnmnt, freq_itemsets, min_cluster_sup);
			if(temp_score>optimum or optimum==-100):
				optimum=temp_score;
				opt_list=clusters;
			else:
				if(temp_score==optimum):
					if(len(clusters)>len(opt_list)):
						opt_list=clusters;
		disjoint_assgnmnt[tuple(opt_list)].append(doc_vectors[0]);
	return disjoint_assgnmnt;



"""
Description :
This function takes the dictionary of disjoint assignment of documents, dictionary of just non-empty clusters with disjoint assignment of documents,
and the global frequent itemsets. The output is the dictionary with the keys as the cluster tuples and the values as the list of documents
which are the all the documents belonging to all descendants of that cluster.
"""
"""
Function        :       find_descendants
Description     :       Find descendants for all disjoint clusters to find cluster frequnet items and cluster support in score calculation.
Input Params    :       Parameter 1 : Disjoint Document Clusters. A dictionary of the form {frequent itemsets, document vectors} 
                        Parameter 2 : Disjoint Document Clusters with non-empty document lists. Dictionary of the form {frequent itemsets, documents assigned}
                        Parameter 3 : A list of global frequent itemsets
Output          :       A dictionary of the form {frequent itemsets, document vectors}
"""
def find_descendants(disjoint_clusters, pruned_clusters, freq_itemsets):
	descendants={};
	for itemsets in freq_itemsets:
		descendants[tuple(itemsets)]=[];
	disjoint_tuples=disjoint_clusters.items();
	pruned_tuples=pruned_clusters.items();
	for pruned_tuple in pruned_tuples:
		terms_in_label=len(pruned_tuple[0]);
		for disjoint_tuple in disjoint_tuples:
			if(len(disjoint_tuple[0])<terms_in_label):
				flag=1;
				for terms in disjoint_tuple[0]:
					if(pruned_tuple[0].count(terms)==0):
						flag=0;
						break;
				if(flag==1):
					for docs in pruned_tuple[1]:
						descendants[disjoint_tuple[0]].append(docs);
	return descendants;


"""
Description :
This function takes the list of documents and the dictionary of feature vectors as input and outputs a conceptual document vector combining feature vectors
of all documents in the list.
"""
"""
Function        :       find_virtual_doc_vector
Description     :       Internal Function
"""
def find_virtual_doc_vector(documents, feature_vectors):
	doc_vector=[];
	for docs in documents:
		for terms in feature_vectors[docs]:
			if(doc_vector.count(terms)==0):
				doc_vector.append(terms);
	return doc_vector;


"""
Description :
This function takes the global frequent itemsets as the input and outputs the maximum depth or level (integer) of the hierarchical tree.
"""
"""
Function        :       find_maximum_level
Description     :       Internal Function
"""
def find_maximum_level(freq_itemsets):
	max_level=-100;
	for itemset in freq_itemsets:
		if(len(itemset)>max_level):
			max_level=len(itemset);
	return max_level;


"""
Description :
This function takes the dictionary of the disjoint clusters and the global frequent itemsets as input and outputs the same dictionary of disjoint clusters
after deleting the empty leaf clusters.
"""
"""
Function        :       delete_empty_leaf_nodes
Description     :       Delete all empty leaf clusters
Input Params    :       Parameter 1 : Disjoint Document Clusters. A dictionary of the form {frequent itemsets, document vectors} 
                        Parameter 2 : A list of global frequent itemsets
Output          :       A dictionary of the form {frequent itemsets, document vectors} with non-empty leaf nodes
"""
def delete_empty_leaf_nodes(disjoint_clusters, freq_itemsets):
	disjoint_tuples=disjoint_clusters.items();
	for disjoint_tuple in disjoint_tuples:
		if(len(disjoint_tuple[0])==find_maximum_level(freq_itemsets) and len(disjoint_tuple[1])==0):
			del disjoint_clusters[disjoint_tuple[0]];
	return disjoint_clusters;


"""
Description :
This function takes the list of terms or labels representing the cluster and global frequent itemsets and outputs the potential parents,
that is the parents lying at a level above and with the cluster label subset of the input cluster. The output is the list of the potential parent clusters.
"""
"""
Function        :       find_potential_parents
Description     :       Internal Function
"""
def find_potential_parents(cluster, freq_itemsets):
	parent_length=len(cluster)-1;
	potential_parents=[];
	for itemset in freq_itemsets:
		if(len(itemset)==parent_length):
			flag=1;
			for terms in itemset:
				if(cluster.count(terms)==0):
					flag=0;
					break;
			if(flag==1):
				potential_parents.append(itemset);
	return potential_parents;


"""
Description :
This function takes conceotual document vector, list of terms representing cluster, dictionary of feature vectors, dictionary of descendants,
global frequent itemsets, minimum cluster support and documents from which the conceptual document vector was formed. The output is the score.
"""
"""
Function        :       get_score_tree
Description     :       Internal Function
"""
def get_score_tree(doc_vector, cluster, feature_vectors, descendants, freq_itemsets, minsup, documents):
	score=0;
	support=0;
	for terms in doc_vector:
                support=find_cluster_support(terms, cluster, descendants, feature_vectors);
		if(support<minsup):
			support=(-1)*find_global_support(terms, freq_itemsets, feature_vectors);	
		score=score+support*find_tf_idf_tree(terms, documents);
	return score;

"""
Description :
This fucntion takes the term and list of documents as the output and outputs (sum of TFs in each document)*idf.
"""
"""
Function        :       find_tf_idf_tree
Description     :       Internal Function
"""
def find_tf_idf_tree(term, document):
	fp=open('input.txt','r');
	no_of_docs=len(fp.read().splitlines());
	fp.close();
	fp=open('postingslist.txt','r');
	entries=fp.read().splitlines();
	fp.close();
	tf=0;
	count=0;
	for entry in entries:
		vector=entry.split(' ');
		word=vector[0];
		doc=vector[1];
		freq=vector[2];
		if(term == word):
			count=count+1;
			if(document.count(doc)>0):
				tf=tf+float(freq);
	idf=math.log(float(no_of_docs)/(float(count)+1));
	return float(tf*idf);



"""
Description :
This function takes the global frequent itemsets, feature vectors, descendants and the dictionary of disjoint clusters.
The output is the list of 2 dictionaries tree and sub_tree. Tree has cluster tuples as keys and its parent cluster as values.
Sub_tree maintains the list of documents lying under that tree considering that cluster as the root of the tree.
"""
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
def build_tree(freq_itemsets, feature_vectors, descendants, disjoint_clusters, minsup):
	sub_tree={};
	tree={};
	output=[];
	disjoint_tuples=disjoint_clusters.items();
	for disjoint_tuple in disjoint_tuples:
		sub_tree[disjoint_tuple[0]]=[];
		tree[disjoint_tuple[0]]=[];
		for docs in disjoint_tuple[1]:
			sub_tree[disjoint_tuple[0]].append(docs);
	level=find_maximum_level(freq_itemsets);
	while(level>=2):
		for disjoint_tuple in disjoint_tuples:
			if(len(disjoint_tuple[0])==level):
				potential_parents=find_potential_parents(list(disjoint_tuple[0]),freq_itemsets);
				virtual_doc_vector=find_virtual_doc_vector(sub_tree[disjoint_tuple[0]], feature_vectors);
				opt_score=-100;
				opt_parent=[];
				temp_score=0;
				temp_parent=[];
				for parents in potential_parents:
					temp_score=get_score_tree(virtual_doc_vector, parents, feature_vectors, descendants, freq_itemsets, minsup, sub_tree[disjoint_tuple[0]]);
					if(temp_score>opt_score):
						opt_parent=parents;
						opt_score=temp_score;
				tree[disjoint_tuple[0]]=opt_parent;
				for docs in disjoint_tuple[1]:
					if(sub_tree[tuple(opt_parent)].count(docs)==0):
						sub_tree[tuple(opt_parent)].append(docs);
		level=level-1;
	output.append(tree);
	output.append(sub_tree);
	return output;


"""
Description :
This function takes the document vector and the list of documents and gives the denominator or the normalizer of the similarity measure.
"""
"""
Function        :       find_normalizer
Description     :       Internal Function
"""
def find_normalizer(doc_vector, documents):
	weight=0;
	for terms in doc_vector:
		weight=weight+find_tf_idf_tree(terms,documents);
	return weight;



"""
Description :
This function the old parent with the new parent in the tree dictionary.
"""
"""
Function        :       change_parent
Description     :       Internal Function
"""
def change_parent(old_parent, new_parent, tree):
	tree_tuples=tree.items();
	for tree_tuple in tree_tuples:
		if(tree[tree_tuple[0]]==list(old_parent)):
			tree[tree_tuple[0]]=list(new_parent);
	#if(tree.has_key(tuple(old_parent))):
	#	del tree[tuple(old_parent)];
	return tree;

"""
Description :
This function takes the dictionary of disjoint clusters, dictionary of tree, dictionary of sub_tree, global frequent itemsets,
feature vectors and dictionary of descendants. The output is list of three dictionaries: disjoint clusters, tree and sub_tree.
"""
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
def child_prune(disjoint_clusters, tree, sub_tree, freq_itemsets, feature_vectors, descendants, minsup):
	output=[];
	level=find_maximum_level(freq_itemsets);
	disjoint_tuples=disjoint_clusters.items();
	while(level>=2):
		for disjoint_tuple in disjoint_tuples:
			if(len(disjoint_tuple[0])==level):
				virtual_doc_vector1=find_virtual_doc_vector(sub_tree[disjoint_tuple[0]], feature_vectors);
				score1=get_score_tree(virtual_doc_vector1, tree[disjoint_tuple[0]], feature_vectors, descendants, freq_itemsets, minsup, sub_tree[disjoint_tuple[0]]);
				normalizer1=find_normalizer(virtual_doc_vector1,sub_tree[disjoint_tuple[0]]);
				virtual_doc_vector2=find_virtual_doc_vector(sub_tree[tuple(tree[disjoint_tuple[0]])], feature_vectors);
				score2=get_score_tree(virtual_doc_vector2, list(disjoint_tuple[0]), feature_vectors, descendants, freq_itemsets, minsup, sub_tree[tuple(tree[disjoint_tuple[0]])]);
				normalizer2=find_normalizer(virtual_doc_vector2,sub_tree[tuple(tree[disjoint_tuple[0]])]);
				final_score=((score1/(normalizer1+1))+1)*((score2/(normalizer2+1))+1);
				#print final_score;
				if(final_score>1.0):
					tree=change_parent(disjoint_tuple[0], tuple(tree[disjoint_tuple[0]]), tree);
					for docs in disjoint_tuple[1]:
						if(disjoint_clusters[tuple(tree[disjoint_tuple[0]])].count(docs)==0):
							disjoint_clusters[tuple(tree[disjoint_tuple[0]])].append(docs);
					#Updation
					sub_tree=update_sub_tree(disjoint_tuple[0], tuple(tree[disjoint_tuple[0]]), sub_tree);	
					del disjoint_clusters[disjoint_tuple[0]];
					del tree[disjoint_tuple[0]];
						
					del sub_tree[disjoint_tuple[0]];	
		level=level-1;
	output.append(disjoint_clusters);
	output.append(tree);
	output.append(sub_tree);
	return output;






"""
Description :
This function removes the documents from the old cluster and puts them into the new cluster in the sub_tree dictionary.
"""
"""
Function        :       update_sub_tree
Description     :       Internal Function
"""
def update_sub_tree(old_cluster, new_cluster, sub_tree):
	for docs in sub_tree[old_cluster]:
		if(sub_tree[new_cluster].count(docs)==0):
			sub_tree[new_cluster].append(docs);
		#sub_tree[old_cluster].remove(docs);
	#del sub_tree[old_cluster];
	return sub_tree;


"""
Description :
This function takes the disjoint clusters dictionary, tree dictionary, sub_tree dictionary, global frequent itemsets, feature vectors and descendants dictionary.
The output is list of three dictionaries: disjoint clusters, tree and sub_tree.
"""

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
def merge_siblings(disjoint_clusters, tree, sub_tree, freq_itemsets, feature_vectors, descendants, minsup):
	level_1_clusters=0;
	siblings=[];
	output=[];
	clusters=disjoint_clusters.items();
	for cluster in clusters:
		if(len(cluster[0])==1):
			level_1_clusters=level_1_clusters+1;
			siblings.append(cluster[0]);
	for cluster in siblings:
		if(disjoint_clusters.has_key(cluster)):
			for disjoint_tuple in clusters:
				if(len(disjoint_tuple[0])==1 and not(disjoint_tuple[0]==cluster) and sub_tree.has_key(cluster) and sub_tree.has_key(disjoint_tuple[0])):
					virtual_doc_vector1=find_virtual_doc_vector(sub_tree[disjoint_tuple[0]], feature_vectors);
					score1=get_score_tree(virtual_doc_vector1, list(cluster), feature_vectors, descendants, freq_itemsets, minsup, sub_tree[disjoint_tuple[0]]);
					normalizer1=find_normalizer(virtual_doc_vector1,sub_tree[disjoint_tuple[0]]);
					virtual_doc_vector2=find_virtual_doc_vector(sub_tree[cluster], feature_vectors);
					score2=get_score_tree(virtual_doc_vector2, list(disjoint_tuple[0]), feature_vectors, descendants, freq_itemsets, minsup, sub_tree[cluster]);
					normalizer2=find_normalizer(virtual_doc_vector2,sub_tree[cluster]);
					final_score=((score1/(normalizer1+1))+1)*((score2/(normalizer2+1))+1);
					#print final_score;
					if(final_score>1.0):
						
						tree=change_parent(cluster, disjoint_tuple[0], tree);
						sub_tree=update_sub_tree(cluster, disjoint_tuple[0], sub_tree);
						disjoint_clusters=update_sub_tree(cluster, disjoint_tuple[0], disjoint_clusters);
						
						del tree[cluster];
						del sub_tree[cluster];
						del disjoint_clusters[cluster];
						

	output.append(disjoint_clusters);
	output.append(tree);
	output.append(sub_tree);
	return output;



"""
Function        :       find_cluster_description
Description     :       Describe the clusters labels with corresponding cluster frequent items
Input Params    :       Parameter 1 : Final Tree [0]
                        Parameter 2 : Final Tree [1]
                        Parameter 3 : Final Tree [2]
                        Parameter 4 : Feature Vectors. A dictionary of the form {docname,transaction vector} with transaction vector
                                      having terms with non-zero freqenciesA
                        Parameter 5 : list of global frequent itemsets
                        Parameter 6 : Cluster Minimum support values 
Output          :       Dictionary of cluster label description
"""
def find_cluster_description(disjoint_clusters, tree, sub_tree, feature_vectors, freq_itemsets, minsup):
	cluster_description={};
	freq_items=[];
	for sets in freq_itemsets:
		for terms in sets:
			if(freq_items.count(terms)==0):
				freq_items.append(terms);
	sub_tree_tuples=sub_tree.items();
	for sub_tree_tuple in sub_tree_tuples:
		if(len(sub_tree[sub_tree_tuple[0]])>0):
			cluster_freq=[];
			no_of_docs=len(sub_tree[sub_tree_tuple[0]]);
			for terms in freq_items:
				count=0;
				support=0.0;
				for docs in sub_tree[sub_tree_tuple[0]]:
					if(feature_vectors[docs].count(terms)>0):
						count=count+1;
				support=float(count)/float(no_of_docs);
				if(support>=minsup):
					cluster_freq.append(terms);
			cluster_description[sub_tree_tuple[0]]=cluster_freq;
	return cluster_description;


"""
**********************************************************************************************************************************************************
                                                                        End of Code
**********************************************************************************************************************************************************                                        
"""
