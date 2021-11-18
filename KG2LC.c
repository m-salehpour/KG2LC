#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <gmodule.h>
#include "sddapi.h"
#include <math.h>
#include <time.h>


#define st_size 2500

//#define subject_size   117  //2^7
//#define predicate_size 39
//#define object_size    552

//LUBM 100K  s:17175   p:18  o:13948  t:100545
#define bit_identifier      16  //10-1
#define bit_subject_size    14  //10-1  //2^7
#define bit_predicate_size  14   //10-1
#define bit_object_size     14  //10-1
#define number_of_vars bit_subject_size+bit_predicate_size+bit_object_size+bit_identifier+4



/*
#define bit_identifier      26 //10-1
#define bit_subject_size    26 //10-1  //2^7
#define bit_predicate_size  26 //10-1
#define bit_object_size     26 //10-1
#define number_of_vars bit_subject_size+bit_predicate_size+bit_object_size+bit_identifier+4
*/






long s_counter=0, p_counter=0, o_counter=0;
long line_number=0;



SddLiteral var_count;
int auto_gc_and_minimize;
//const char* type = "balanced";
SddManager* m;

//Vtree* vtree = sdd_vtree_new(var_count,type);
//SddManager* m = sdd_manager_new(vtree);


SddNode* kg;
//SddNode* alpha;
SddNode* _intersection_sdd;
SddNode* tmp_node;

GHashTable *triple_index;
GHashTable *__all_triple_index__;



char *triple_whole_line;



char* dec2bin(int b, int n)
{

        // Size of an integer is assumed to be 'b' bits
        //
        char* res = malloc(number_of_vars+1);
        int pi=0;
        for (int i = b; i >= 0; i--)
        {
                int k = n >> i;
                if (k & 1)
                        res[pi++]='1'; //printf("1");
                else
                        res[pi++]='0';//printf("0");

        }
        res[pi]='\0';

        //printf("\n counter:%d     bin:%s \n",n, res);

        return res;

}



void ___sdd_construct(char* _ss_b, char* _pp_b, char* _oo_b)
{
	SddNode* alpha;
	//sdd_ref(alpha,m);


        char* test = malloc(number_of_vars+1);



        if (_ss_b[0] == '0')
	{
                alpha = sdd_manager_literal(-1,m);
		//sdd_ref(alpha,m);
	}
        else
	{
                alpha = sdd_manager_literal( 1,m);
		//sdd_ref(alpha,m);
	}
	
	//sdd_ref(alpha,m);
	
        test[0] = _ss_b[0];



        for (int i=1; i<=(bit_subject_size);i++)
        {
                if (_ss_b[i] == '0')
                        alpha = sdd_conjoin(alpha,sdd_manager_literal(-(i+1),m),m);
                else
                        alpha = sdd_conjoin(alpha,sdd_manager_literal( (i+1),m),m);

		//sdd_ref(alpha,m);



                test[i] = _ss_b[i];


        }

        for (int i=(bit_subject_size+1); i<=((bit_predicate_size)+(bit_subject_size)+1);i++)
        {
                if (_pp_b[i-(bit_subject_size+1)] == '0')
                        alpha = sdd_conjoin(alpha,sdd_manager_literal(-(i+1),m),m);
                else
                        alpha = sdd_conjoin(alpha,sdd_manager_literal( (i+1),m),m);

		//sdd_ref(alpha,m);


                test[i] = _pp_b[i-(bit_subject_size+1)];

        }

        for (int i=((bit_predicate_size)+(bit_subject_size)+2); i<=((bit_object_size)+(bit_predicate_size)+(bit_subject_size)+2);i++)
        {
                if (_oo_b[i-((bit_predicate_size)+(bit_subject_size)+2)] == '0')
                        alpha = sdd_conjoin(alpha,sdd_manager_literal(-(i+1),m),m);
                else
                        alpha = sdd_conjoin(alpha,sdd_manager_literal( (i+1),m),m);

		//sdd_ref(alpha,m);



                test[i] = _oo_b[i-((bit_predicate_size)+(bit_subject_size)+2)];
		
        }


	char* id = dec2bin(bit_identifier,line_number);

	 for (int i=((bit_predicate_size)+(bit_object_size)+(bit_subject_size)+3); i<=((bit_identifier)+(bit_object_size)+(bit_predicate_size)+(bit_subject_size)+3);i++)
        {
                if (id[i-((bit_predicate_size)+(bit_object_size)+(bit_subject_size)+3)] == '0')
                        alpha = sdd_conjoin(alpha,sdd_manager_literal(-(i+1),m),m);
                else
                        alpha = sdd_conjoin(alpha,sdd_manager_literal( (i+1),m),m);

		//sdd_ref(alpha,m);



                test[i] = id[i-((bit_predicate_size)+(bit_object_size)+(bit_subject_size)+3)];

        }


        test[(number_of_vars)] = '\0';                
        
	kg = sdd_disjoin(kg,alpha,m);
	sdd_ref(kg,m);
	
	//sdd_deref(alpha,m);


	 #if (DEBUG == 10)
        {

        printf("\n triple observed (test): %s \n****************************************************\n ", test);
	
	}
#endif
	free(test);
	//getchar();
	
	g_hash_table_insert(__all_triple_index__, g_strdup (id), g_strdup (triple_whole_line) );
	


}



long long hash_size = 0;

void triple_construct(char _ss[st_size], char _pp[st_size], char _oo[st_size])
{
      
	
	char*  s_key   = NULL;
  	char*  s_value = NULL;
  	char** s_key_ptr   = &s_key;
  	char** s_value_ptr = &s_value;
      
       	char*  p_key   = NULL;
        char*  p_value = NULL;
        char** p_key_ptr   = &p_key;
        char** p_value_ptr = &p_value;

	char*  o_key   = NULL;
        char*  o_value = NULL;
        char** o_key_ptr   = &o_key;
        char** o_value_ptr = &o_value;

	



	
	gboolean ss_found = g_hash_table_lookup_extended(triple_index, _ss, (gpointer*)s_key_ptr, (gpointer*)s_value_ptr);
	gboolean pp_found = g_hash_table_lookup_extended(triple_index, _pp, (gpointer*)p_key_ptr, (gpointer*)p_value_ptr);
	gboolean oo_found = g_hash_table_lookup_extended(triple_index, _oo, (gpointer*)o_key_ptr, (gpointer*)o_value_ptr);
	



	if (!ss_found)
	{
		g_hash_table_insert(triple_index, g_strdup (_ss), g_strdup ( dec2bin(bit_subject_size,s_counter++)));
		hash_size+= strlen(_ss);
		hash_size+= strlen(dec2bin(bit_subject_size,s_counter-1));
	}

	if (!pp_found)
	{

		g_hash_table_insert(triple_index, g_strdup (_pp), g_strdup (dec2bin(bit_predicate_size,s_counter++)));
                hash_size+= strlen(_pp);
  		hash_size+= strlen(dec2bin(bit_subject_size,s_counter-1));

	}

	if (!oo_found)
	{
		g_hash_table_insert(triple_index, g_strdup (_oo), g_strdup (dec2bin(bit_object_size,s_counter++) ));
                hash_size+= strlen(_oo);
                hash_size+= strlen(dec2bin(bit_subject_size,s_counter-1));

	}




	g_hash_table_lookup_extended(triple_index, _ss, (gpointer*)s_key_ptr, (gpointer*)s_value_ptr);
        g_hash_table_lookup_extended(triple_index, _pp, (gpointer*)p_key_ptr, (gpointer*)p_value_ptr);
        g_hash_table_lookup_extended(triple_index, _oo, (gpointer*)o_key_ptr, (gpointer*)o_value_ptr);




	#if (DEBUG == 10)  
        {
	printf("There are %u keys in the KV hash\n", g_hash_table_size(triple_index));

	printf("SS: Found that binary of %s is %s\n", s_key, s_value);
	printf("PP: Found that binary of %s is %s\n", p_key, p_value);
	printf("OO: Found that binary of %s is %s\n", o_key, o_value);
	}
	#endif

	 #if (DEBUG == 919)
        {
        printf("There are %u keys in the KV hash\n", g_hash_table_size(triple_index));

        printf("SS: Found that binary of %s is %s\n", s_key, _ss);
        printf("PP: Found that binary of %s is %s\n", p_key, _pp);
        printf("OO: Found that binary of %s is %s\n", o_key, _oo);
        }
        #endif


		
	size_t line_length = strlen(_ss) + strlen(_pp) + strlen(_oo) + 3 ;
	char *w_space = " ";
	triple_whole_line = (char*)malloc(line_length);

	strcpy(triple_whole_line,_ss);
	strcat(triple_whole_line,w_space);
	strcat(triple_whole_line,_pp);
        strcat(triple_whole_line,w_space);
	strcat(triple_whole_line,_oo);


	___sdd_construct(s_value, p_value, o_value);
	free(triple_whole_line);







}

FILE *fp = NULL;

void main_kb (char *argv)
{
	  char subject[st_size], tr_predicate[st_size], object[st_size];
	  int readfile = 0;
	  //size_t line_number=0;
	  bool flag_bnode = false;
  	  char *check_ws;
  	  char gt = '>';
  	  long fpos;
  

	
	  
  	  fp = fopen(argv, "r");
  	  if (fp == NULL)
  	  {
  		  printf ("Error opening the file\n\n");
  		  exit(EXIT_FAILURE);
  	  } else
  	  {
		  while (readfile != EOF )//&& line_number < 1001)
		  {
			  line_number++;
                          printf("\n line:     %ld \n hash_size:  %lld  \n\n", line_number, hash_size);


			       #if (DEBUG == 5)
                                  {
       	
					  
	
					  if (line_number % 1000 == 0)
					  {
						  sdd_manager_print(m);
      						  printf("\n line:     %d \n hash_size:  %d  \n\n", line_number, hash_size);
						  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(kg) ? "F":"T");
						  SddModelCount count33  = sdd_model_count(kg,m);
						  SddModelCount gcount33 = sdd_global_model_count(kg,m);
						  printf("\n sdd query model count       : %"PRImcS"\n",count33);
						  printf("\n sdd global model count       : %"PRImcS"\n",gcount33);


					  }

					  


                                  }
                                    #endif


			  fpos = ftell(fp);
  			  readfile = fscanf(fp, "%s %s %[^\n]\n", subject, tr_predicate, object);
  
			  if (subject[0] == '_' && subject[1]==':')
  				  flag_bnode = true;
  			  else
  				  check_ws = strstr(subject,">");
  
			  if (!flag_bnode && check_ws == NULL)
  			  {
				fseek(fp, fpos, SEEK_SET);
				readfile = fscanf(fp, "%[^>]> %s %[^\n]\n", subject, tr_predicate, object);
				strncat(subject, &gt, 1);
			
			  }
  			  flag_bnode = false;

			  object[(strlen(object)-2)]='\0';

			  if (readfile == 3)
			  {
				    #if (DEBUG == 11)  
				  {
  				
					  printf("line:%zu \n s: %s \n  p:%s \n o:%s \n \n\n\n ", line_number, subject, tr_predicate, object);
					  printf("s_l: %d \t  p_l:%d \t o_l:%d \n ", strlen(subject), strlen(tr_predicate), strlen(object));

				
				  }
				    #endif

				  triple_construct(subject , tr_predicate, object);


			  }


		  }
	  
	  
	  }




}

void join_sdd(SddNode* left_sdd_, SddNode* right_sdd_, int bit_start, int bit_end)
{
	SddNode* tmp_sdd;
	SddNode* tmp_sdd2_;
	SddModelCount count;
	SddNode* _join_sdd = sdd_manager_false(m);
	_intersection_sdd = sdd_manager_false(m);
	char char_tmp_var[number_of_vars];
	int start_size = 2;
	bool join_intersection = false;
	
	int* _int_models_var = (int*)calloc(start_size, sizeof(int));
	size_t _index_model_vars=0;

	

	for(size_t i=0; i<pow(2,((bit_end-bit_start)+1)); i++ )
	{
		tmp_sdd = left_sdd_;
		strcpy(char_tmp_var,dec2bin((bit_end-bit_start),i));

		//printf("\n binary: %s ", char_tmp_var);

		for(int j=bit_start; j<=bit_end; j++)
		{
			//printf("\n j-bin start: %d ", (j-bit_start));

			if (char_tmp_var[(j-bit_start)] == '0')
                                tmp_sdd = sdd_condition( -j, tmp_sdd, m  );
                        else
                                tmp_sdd = sdd_condition(  j, tmp_sdd, m  );


                }//subset search space for either S, P, or O

		if(!sdd_node_is_false(tmp_sdd))
		{
			_join_sdd = sdd_disjoin(_join_sdd,tmp_sdd,m);
			_int_models_var[_index_model_vars++] = i;
			_int_models_var = (int*)realloc(_int_models_var, ++start_size * sizeof(int));
			/*
			 int* sdd_v2 = sdd_variables(_join_sdd, m);
		       
			 for (size_t ii=1; ii< number_of_vars+1 ; ii++ )
		       		 printf("\n var %d: %d", ii, sdd_v2[ii]);

			*/

			


		}


	}//whole variable search space	


	printf("\n\n ******************************** \n\n");
	printf(" observation?  : %s\n",  sdd_node_is_false(_join_sdd) ? "F":"T");
	SddModelCount count_2  = sdd_model_count(_join_sdd,m);
	printf(" Left sdd query model count       : %"PRImcS"\n",count_2);
        printf(" ******************************** \n");




	for(size_t i=0; i< _index_model_vars;i++)
	{

		//printf("\n ones: %d", _int_models_var[i]);
	        tmp_sdd   = right_sdd_;
		tmp_sdd2_ = _join_sdd;
                strcpy(char_tmp_var,dec2bin((bit_end-bit_start), _int_models_var[i]  ));

                for(int j=bit_start; j<=bit_end; j++)
                {
                        if (char_tmp_var[(j-bit_start)] == '0')
			{
                                tmp_sdd   = sdd_condition( -j, tmp_sdd   , m  );
				tmp_sdd2_ = sdd_condition( -j, tmp_sdd2_ , m  );
			}
                        else
			{
                                tmp_sdd   = sdd_condition(  j, tmp_sdd   , m  );
				tmp_sdd2_ = sdd_condition(  j, tmp_sdd2_ , m  );

			}


                }//subset search space for either S, P, or O

                if(!sdd_node_is_false(tmp_sdd))
                {
			join_intersection = true;
                        _intersection_sdd = sdd_disjoin(_intersection_sdd, tmp_sdd   ,m);
			_intersection_sdd = sdd_disjoin(_intersection_sdd, tmp_sdd2_ ,m);

                        //_int_models_var[_index_model_vars++] = i;
                        //_int_models_var = (int*)realloc(_int_models_var, ++start_size * sizeof(int));
                }

	}

	if (!join_intersection) //in case there is no info in common to join
		 _intersection_sdd = sdd_conjoin(_intersection_sdd , sdd_manager_false(m) ,m);

	printf("\n\n ******************************** \n\n");
	printf(" observation?  : %s\n",  sdd_node_is_false( _intersection_sdd ) ? "F":"T");
	count  = sdd_model_count( _intersection_sdd ,m);
	printf(" Intersection (join) sdd query model count       : %"PRImcS"\n",count);
        printf(" ******************************** \n");



	printf("\n End of Var Exists!\n");

}

int index_model_vars_g = 0;
int model_vars_g[1000];


void sdd_split(int bit_start, int bit_end, SddNode* relation_sdd)
{

	char char_tmp_var[number_of_vars];
	SddNode* tmp_sdd_local;
	index_model_vars_g = 0;
	//int start_size_l = 1;
	//model_vars[0] = (int*)calloc(start_size_l, sizeof(int));

	for(size_t i=0; i<pow(2,((bit_end - bit_start)+1)); i++ )
        {
                tmp_sdd_local = relation_sdd;
                strcpy(char_tmp_var,dec2bin((bit_end - bit_start),i));

                //printf("\n binary: %s ", char_tmp_var);

                for(int j=bit_start; j<=bit_end; j++)
                {
                        //printf("\n j-bin start: %d ", (j-bit_start));

                        if (char_tmp_var[(j-bit_start)] == '0')
                                tmp_sdd_local = sdd_condition( -j, tmp_sdd_local, m  );
                        else
                                tmp_sdd_local = sdd_condition(  j, tmp_sdd_local, m  );


                }//subset search space for either S, P, or O

                if(!sdd_node_is_false(tmp_sdd_local))
                {
                       //_join_sdd = sdd_disjoin(_join_sdd,tmp_sdd,m);
                        model_vars_g[(index_model_vars_g)++] = i;
                        //model_vars[(index_model_vars)]= (int*)realloc(model_vars, start_size_l * sizeof(int));
                        /*
                         int* sdd_v2 = sdd_variables(_join_sdd, m);

                         for (size_t ii=1; ii< number_of_vars+1 ; ii++ )
                                 printf("\n var %d: %d", ii, sdd_v2[ii]);

                        */




                        //char* s_key = NULL;
                        //char* s_value = NULL;
                        //char** s_key_ptr   = &s_key;
                        //char** s_value_ptr = &s_value;
                        //gboolean ss_found = g_hash_table_lookup_extended(s_binary_index, dec2bin((bit_end-bit_start),i) , (gpointer*)s_key_ptr, (gpointer*)s_value_ptr);

                        //if(ss_found)
                        //      printf("SS: Found that binary of %s is %s\n", s_key, s_value);



                        //printf(" observation %d?  : %s ", i, sdd_node_is_false(tmp_sdd) ? "F":"T");
                        //SddModelCount count_1  = sdd_model_count(tmp_sdd,m);
                        //kk++; //+= count_1;
                        //printf("\n %d sdd query model count       : %"PRImcS"\n",kk, count_1);
                }


        }//whole variable search space

	printf("\n**********************************************************\n");
	printf("found: %d", (index_model_vars_g));
        printf("\n**********************************************************\n");


}

void sdd_match(int bit_start, int bit_end, SddNode* relation_sdd)
{


	printf("\n Start Match!\n");


	SddNode* tmp_sdd;
	//SddNode* tmp_sdd2_;
	char char_tmp_var[number_of_vars];
	//bool join_intersection = false;
	//SddNode* _intersection_sdd;

	       
       	for(int i=0; i< (index_model_vars_g);i++)
        {

                printf("\n ones: %d", model_vars_g[i] );
                tmp_sdd   = relation_sdd;
                //tmp_sdd2_ = _join_sdd;
		//
                strcpy(char_tmp_var,dec2bin((bit_end - bit_start), model_vars_g[i]  ));

                for(int j=bit_start; j<=bit_end; j++)
                {
                        if (char_tmp_var[(j-bit_start)] == '0')
                        {
                                tmp_sdd   = sdd_condition( -j, tmp_sdd   , m  );
                                //tmp_sdd2_ = sdd_condition( -j, tmp_sdd2_ , m  );
                        }
                        else
                        {
                                tmp_sdd   = sdd_condition(  j, tmp_sdd   , m  );
                                //tmp_sdd2_ = sdd_condition(  j, tmp_sdd2_ , m  );

                        }

			printf("\t j: %d", j);


                }//subset search space for either S, P, or Oi

                /*

                for(int j=l_bit_start; j<=l_bit_end; j++)
                {
                        if (char_tmp_var[(j-l_bit_start)] == '0')
                        {
                                //tmp_sdd   = sdd_condition( -j, tmp_sdd   , m  );
                                tmp_sdd2_ = sdd_condition( -j, tmp_sdd2_ , m  );
                        }
                        else
                        {
                                //tmp_sdd   = sdd_condition(  j, tmp_sdd   , m  );
                                tmp_sdd2_ = sdd_condition(  j, tmp_sdd2_ , m  );

                        }


                }//subset search space for either S, P, or O
                */




                if(!sdd_node_is_false(tmp_sdd))
                {
			/*

                        for(int j=l_bit_start; j<=l_bit_end; j++)
                        {
                                if (char_tmp_var[(j-l_bit_start)] == '0')
                                {
                                        //tmp_sdd   = sdd_condition( -j, tmp_sdd   , m  );
                                        tmp_sdd2_ = sdd_condition( -j, tmp_sdd2_ , m  );

                                }
                                else
                                {
                                        //tmp_sdd   = sdd_condition(  j, tmp_sdd   , m  );
                                        tmp_sdd2_ = sdd_condition(  j, tmp_sdd2_ , m  );
                                }
                        }//subset search space for either S, P, or O

                        join_intersection = true;
                        _intersection_sdd = sdd_disjoin(_intersection_sdd, tmp_sdd   ,m);
                        _intersection_sdd = sdd_disjoin(_intersection_sdd, tmp_sdd2_ ,m);
			*/

                        printf("\n here \n");

                        //_int_models_var[_index_model_vars++] = i;
                        //_int_models_var = (int*)realloc(_int_models_var, ++start_size * sizeof(int));
                }

        }

	/*
        if (!join_intersection) //in case there is no info in common to join
                 _intersection_sdd = sdd_conjoin(_intersection_sdd , sdd_manager_false(m) ,m);

        printf("\n\n ******************************** \n\n");
        printf(" observation?  : %s\n",  sdd_node_is_false( _intersection_sdd ) ? "F":"T");
        SddModelCount count_3  = sdd_model_count( _intersection_sdd ,m);
        printf(" Intersection (join) sdd query model count       : %"PRImcS"\n",count_3);
        printf(" ******************************** \n");

	*/


        printf("\n End of Var Exists!\n");

	printf("\n End Match!\n");





}

int bin_dec(char* bin_str) 
{
    	//char *bin_str = "11001";

    	unsigned result = 0;
    	while (*bin_str)
	{
		result *= 2;
		result += *bin_str == '1' ? 1 : 0;
		++bin_str;
    	}
    	printf("%d\n", result);
    	return result;
}


void join_sdd2(SddNode* left_sdd_, SddNode* right_sdd_, int l_bit_start, int l_bit_end,int r_bit_start, int r_bit_end)
{
        SddNode* tmp_sdd;
	SddNode* tmp_sdd3;
        SddNode* tmp_sdd2_;
        //SddModelCount count;
        SddNode* _join_sdd = sdd_manager_false(m);
        _intersection_sdd = sdd_manager_false(m);
        char char_tmp_var[number_of_vars];
	//char char_tmp_var2[number_of_vars];

        int start_size = 2;
        bool join_intersection = false;
        //int kk=0;

        int* _int_models_var = (int*)calloc(start_size, sizeof(int));
        size_t _index_model_vars=0;



        for(size_t i=0; i<pow(2,((l_bit_end-l_bit_start)+1)); i++ )
        {
                tmp_sdd = left_sdd_;
                strcpy(char_tmp_var,dec2bin((l_bit_end-l_bit_start),i));

                //printf("\n binary: %s ", char_tmp_var);

                for(int j=l_bit_start; j<=l_bit_end; j++)
                {
                        //printf("\n j-bin start: %d ", (j-bit_start));

                        if (char_tmp_var[(j-l_bit_start)] == '0')
                                tmp_sdd = sdd_condition( -j, tmp_sdd, m  );
                        else
                                tmp_sdd = sdd_condition(  j, tmp_sdd, m  );


                }//subset search space for either S, P, or O

                if(!sdd_node_is_false(tmp_sdd))
                {
                        _join_sdd = sdd_disjoin(_join_sdd,tmp_sdd,m);

                        _int_models_var[_index_model_vars++] = i;
                        _int_models_var = (int*)realloc(_int_models_var, ++start_size * sizeof(int));


			#if (DEBUG==1)
			{
			printf("\n i: %d", i);
                        
                         int* sdd_v2 = sdd_variables(_join_sdd, m);
                       
                         for (size_t ii=1; ii< number_of_vars+1 ; ii++ )
			 {
				 //char_tmp_var2[i] = sdd_v2[ii];
                                 printf("\n var %d: %d", ii, sdd_v2[ii]);
			 }
			}
			#endif



                        




                        //char* s_key = NULL;
                        //char* s_value = NULL;
                        //char** s_key_ptr   = &s_key;
                        //char** s_value_ptr = &s_value;
                        //gboolean ss_found = g_hash_table_lookup_extended(s_binary_index, dec2bin((bit_end-bit_start),i) , (gpointer*)s_key_ptr, (gpointer*)s_value_ptr);

                        //if(ss_found)
                        //      printf("SS: Found that binary of %s is %s\n", s_key, s_value);



                        //printf(" observation %d?  : %s ", i, sdd_node_is_false(tmp_sdd) ? "F":"T");
                        //SddModelCount count_1  = sdd_model_count(tmp_sdd,m);
                        //kk++; //+= count_1;
                        //printf("\n %d sdd query model count       : %"PRImcS"\n",kk, count_1);
                }


        }//whole variable search space  


        printf("\n\n ******************************** \n\n");
        printf(" observation?  : %s\n",  sdd_node_is_false(_join_sdd) ? "F":"T");
        SddModelCount count_2  = sdd_model_count(_join_sdd,m);
        printf(" Left sdd query model count       : %"PRImcS"\n",count_2);
        printf(" ******************************** \n");




        for(size_t i=0; i< _index_model_vars;i++)
        {

                //printf("\n ones: %d", _int_models_var[i]);
                tmp_sdd3   = right_sdd_;
                tmp_sdd2_ = _join_sdd;
                strcpy(char_tmp_var,dec2bin((r_bit_end-r_bit_start), _int_models_var[i]  ));
		//printf("\t char_ones: %s",char_tmp_var );

                for(int j=r_bit_start; j<=r_bit_end; j++)
                {
                        if (char_tmp_var[(j-r_bit_start)] == '0')
                        {
                                tmp_sdd3   = sdd_condition( -j, tmp_sdd3   , m  );
                                //tmp_sdd2_ = sdd_condition( -j, tmp_sdd2_ , m  );
                        }
                        else
                        {
                                tmp_sdd3   = sdd_condition(  j, tmp_sdd3   , m  );
                                //tmp_sdd2_ = sdd_condition(  j, tmp_sdd2_ , m  );

                        }


                }//subset search space for either S, P, or Oi

	

		/*
		for(int j=l_bit_start; j<=l_bit_end; j++)
                {
                        if (char_tmp_var[(j-l_bit_start)] == '0')
                        {
                                //tmp_sdd   = sdd_condition( -j, tmp_sdd   , m  );
                                tmp_sdd2_ = sdd_condition( -j, tmp_sdd2_ , m  );
                        }
                        else
                        {
                                //tmp_sdd   = sdd_condition(  j, tmp_sdd   , m  );
                                tmp_sdd2_ = sdd_condition(  j, tmp_sdd2_ , m  );

                        }


                }//subset search space for either S, P, or O
		*/


		/*
		 int* sdd_v3 = sdd_variables(tmp_sdd3, m);

                         for (size_t ii=1; ii< number_of_vars+1 ; ii++ )
                                 printf("\n var %d: %d", ii, sdd_v3[ii]);


		 */
		




                if(!sdd_node_is_false(tmp_sdd3))
                {
	
			for(int j=l_bit_start; j<=l_bit_end; j++)
			{
				if (char_tmp_var[(j-l_bit_start)] == '0')
				{
					//tmp_sdd   = sdd_condition( -j, tmp_sdd   , m  );
					tmp_sdd2_ = sdd_condition( -j, tmp_sdd2_ , m  );
                        
				}
				else
				{
					//tmp_sdd   = sdd_condition(  j, tmp_sdd   , m  );
					tmp_sdd2_ = sdd_condition(  j, tmp_sdd2_ , m  );
				}
			}//subset search space for either S, P, or O

                        join_intersection = true;
                        _intersection_sdd = sdd_disjoin(_intersection_sdd, tmp_sdd3   ,m);
                        //_intersection_sdd = sdd_disjoin(_intersection_sdd, tmp_sdd2_ ,m);

			//printf("\n here \n");

                        //_int_models_var[_index_model_vars++] = i;
                        //_int_models_var = (int*)realloc(_int_models_var, ++start_size * sizeof(int));
                }

        }

        if (!join_intersection) //in case there is no info in common to join
                 _intersection_sdd = sdd_conjoin(_intersection_sdd , sdd_manager_false(m) ,m);

        printf("\n\n ******************************** \n\n");
        printf(" observation?  : %s\n",  sdd_node_is_false( _intersection_sdd ) ? "F":"T");
        SddModelCount count_3  = sdd_model_count( _intersection_sdd ,m);
        printf(" Intersection (join) sdd query model count       : %"PRImcS"\n",count_3);
        printf(" ******************************** \n");



        printf("\n End of Var Exists!\n");
	//char_tmp_var=NULL;
	//free(char_tmp_var2);

}

void decompile__print__triples (SddNode* _input_sdd_, int bit_start, int bit_end)
{
	
        SddNode* tmp_sdd;
	/*
        SddNode* tmp_sdd3;
        SddNode* tmp_sdd2_;
        SddModelCount count;
        SddNode* _join_sdd = sdd_manager_false(m);
        SddNode* _intersection_sdd = sdd_manager_false(m);
	*/
        char char_tmp_var[number_of_vars];
	
        //char char_tmp_var2[number_of_vars];

	/*
        int start_size = 2;
        bool join_intersection = false;
        int kk=0;

        int* _int_models_var = (int*)calloc(start_size, sizeof(int));
        size_t _index_model_vars=0;
	*/



        for(size_t i=0; i<pow(2,((bit_end-bit_start)+1)); i++ )
        {
                tmp_sdd = _input_sdd_;
                strcpy(char_tmp_var,dec2bin((bit_end-bit_start),i));

                //printf("\n binary: %s ", char_tmp_var);

                for(int j=bit_start; j<=bit_end; j++)
                {
                        //printf("\n j-bin start: %d ", (j-bit_start));

                        if (char_tmp_var[(j-bit_start)] == '0')
                                tmp_sdd = sdd_condition( -j, tmp_sdd, m  );
                        else
                                tmp_sdd = sdd_condition(  j, tmp_sdd, m  );


                }//subset search space for either S, P, or O

                if(!sdd_node_is_false(tmp_sdd))
                {
			/*
                        _join_sdd = sdd_disjoin(_join_sdd,tmp_sdd,m);

                        _int_models_var[_index_model_vars++] = i;
                        _int_models_var = (int*)realloc(_int_models_var, ++start_size * sizeof(int));
			*/

			//**************************************************************************************
			
			char*  t_key = NULL;
			char*  t_value = NULL;
			char** t_key_ptr   = &t_key;
			char** t_value_ptr = &t_value;
		
			gboolean triple_found = g_hash_table_lookup_extended(__all_triple_index__ ,char_tmp_var, (gpointer*)t_key_ptr, (gpointer*)t_value_ptr);

			if (triple_found)
				printf(" %s \n", t_value);



			//**************************************************************************************



                        //char* s_key = NULL;
                        //char* s_value = NULL;
                        //char** s_key_ptr   = &s_key;
                        //char** s_value_ptr = &s_value;
                        //gboolean ss_found = g_hash_table_lookup_extended(s_binary_index, dec2bin((bit_end-bit_start),i) , (gpointer*)s_key_ptr, (gpointer*)s_value_ptr);

                        //if(ss_found)
                        //      printf("SS: Found that binary of %s is %s\n", s_key, s_value);



                        //printf(" observation %d?  : %s ", i, sdd_node_is_false(tmp_sdd) ? "F":"T");
                        //SddModelCount count_1  = sdd_model_count(tmp_sdd,m);
                        //kk++; //+= count_1;
                        //printf("\n %d sdd query model count       : %"PRImcS"\n",kk, count_1);
                }


        }//whole variable search space  


}

void lubm_q1();
void lubm_q2();
void lubm_q3();
void lubm_q4();
void lubm_q5();
void lubm_q6();
void lubm_q7();
//void lubm_q72();
//
//
void sp2_q1();
void sp2_q2();
void sp2_q3();
void sp2_q4();

int main(int argc) 
{

	double time_spent=0;
	double total_time = 0.0;
	clock_t clk_begin;
	clock_t clk_end;

	
	 

  // set up vtree and manager
  var_count = number_of_vars;
  auto_gc_and_minimize = 1;
  
  //const char* type = "balanced";
  m = sdd_manager_create(var_count,auto_gc_and_minimize);
 

  if (argc < 2)
  {
  kg = sdd_manager_false(m); 
  sdd_ref(kg,m);
  
  //Vtree* vtree = sdd_vtree_new(var_count,type);
  //SddManager* m = sdd_manager_new(vtree);
  //




  triple_index          = g_hash_table_new(g_str_hash, g_str_equal);
  __all_triple_index__  = g_hash_table_new(g_str_hash, g_str_equal);





  printf("\n***************************  START **********************");
  

  
  //main_kb("/home/ubuntu/www_data/lubm_100_sorted.nt");

  main_kb("/home/ubuntu/www_data/sp2b_100k_sorted.nt");

  
  sdd_ref(kg,m);
  fclose (fp);
  g_hash_table_destroy(triple_index); // just for construction is needed

  printf("\n***************************  END:) **********************");


  printf("saving sdd and vtree ... ");
  sdd_save("output/kgsddsp22.sdd",kg);
  //sdd_save_as_dot("output/dot/kgsdd.dot",kg);
  //sdd_vtree_save_as_dot("output/vtree.dot",vtree);
  printf("done\n");
  // g_hash_table_destroy(triple_index);
  g_hash_table_destroy(__all_triple_index__);// for decompilation is needed




 
 
  }
  else
  {
	  printf("reading sdd from file ...\n");

	  clk_begin = clock();////// Time start


	  //kg = sdd_read("output/kgsdd.sdd.new",m); //LUBM 100K
	  kg = sdd_read("output/kgsddsp22.sdd",m);    //SP2B 100K

	  clk_end = clock();//////// Time end
	  time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
	  printf("Time elpased is %f seconds for reading from SDD \n", time_spent);//Print!!!!

	  sdd_ref(kg, m); // Ref for not being collected

	  
  

  }

/*

  sdd_manager_auto_gc_and_minimize_off(m);
  lubm_q1();
  sdd_manager_garbage_collect(m);
	  
  lubm_q2();
  sdd_manager_garbage_collect(m);

  lubm_q3();
  sdd_manager_garbage_collect(m);

  lubm_q4();
  sdd_manager_garbage_collect(m);

  lubm_q5();
  sdd_manager_garbage_collect(m);

  lubm_q6();
  sdd_manager_garbage_collect(m);

  //lubm_q7();
  //sdd_manager_garbage_collect(m);
*/


  sdd_manager_auto_gc_and_minimize_off(m);

  sp2_q1();
  sdd_manager_garbage_collect(m);

  printf("\nQ1 finish give me char.....\n");
  getchar();

  
  sp2_q2();
  sdd_manager_garbage_collect(m);

  printf("Q2 finish give me char.....\n");
  getchar();

  

  sp2_q3();
  sdd_manager_garbage_collect(m);

  printf("Q3 finish give me char.....\n");
  getchar();

  sp2_q4();
  sdd_manager_garbage_collect(m);


  printf("Q4 (last) finish give me char.....\n");
  getchar();
  
























  //

  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////











 #if (DEBUG == 2)  
  {

  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(kg) ? "F":"T");
  SddModelCount count  = sdd_model_count(kg,m);
  SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",count);
  printf("\n sdd global model count       : %"PRImcS"\n",gcount);


  printf("number of vars: %d\n", number_of_vars);
  
  printf("\n== before referencing:\n");
  printf("  sdd size = %zu\n", sdd_manager_size(m));
  printf("  sdd count = %zu\n", sdd_manager_count(m));
  printf("  live sdd size = %zu\n", sdd_manager_live_size(m));
  printf("  dead sdd size = %zu\n", sdd_manager_dead_size(m));
  sdd_manager_print(m);
  printf("sdd var count       : %"PRIlitS"\n",sdd_manager_var_count(m));
  printf("sdd min cardinality       : %"PRIlitS"\n",sdd_minimum_cardinality(kg));
/*

  // ref SDDs so that they are not garbage collected
  sdd_ref(kg,m);
  sdd_manager_minimize(m);
  printf("\n\n\n\n\n== after referencing:\n");
  printf("  sdd size = %zu\n", sdd_manager_size(m));
  printf("  sdd count = %zu\n", sdd_manager_count(m));
  printf("  live sdd size = %zu\n", sdd_manager_live_size(m));
  printf("  dead sdd size = %zu\n", sdd_manager_dead_size(m));
  sdd_manager_print(m);
  printf("sdd var count       : %"PRIlitS"\n",sdd_manager_var_count(m));
  printf("sdd min cardinality       : %"PRIlitS"\n",sdd_minimum_cardinality(kg));
  */
  }
#endif


 ////////// CLEAN UP //////////
  //

  sdd_deref(kg,m);
  sdd_manager_garbage_collect(m);
  sdd_manager_free(m);

    return 0;
}


void sp2_q4()
{

       	printf("\n######################################################################\n");
        printf("\n********************  SP2Q4                                         \n");
        double time_spent=0;
        double total_time = 0.0;
        clock_t clk_begin;
        clock_t clk_end;



  SddNode* q1;

  //Not Avilable!!!  

  q1 = sdd_condition(  1,  kg, m  );
  q1 = sdd_condition(  2,  q1, m  );
  q1 = sdd_condition(  3,  q1, m  );
  q1 = sdd_condition(  4,  q1, m  );
  q1 = sdd_condition(  5,  q1, m  );
  q1 = sdd_condition(  6,  q1, m  );
  q1 = sdd_condition(  7,  q1, m  );
  q1 = sdd_condition(  11, q1, m  );
  q1 = sdd_condition(  12, q1, m  );
  q1 = sdd_condition(  13, q1, m  );
  q1 = sdd_condition(  14, q1, m  );
  q1 = sdd_condition(  15, q1, m  );



  //<http://www.w3.org/1999/02/22-rdf-syntax-ns#type> is 000000000000111

  q1 = sdd_condition(  -16, q1, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(  -27, q1, m  );
  q1 = sdd_condition(   28, q1, m  );
  q1 = sdd_condition(   29, q1, m  );
  q1 = sdd_condition(   30, q1, m  );



//binary of <http://xmlns.com/foaf/0.1/Person> is 000000000000100
  
  q1 = sdd_condition( -31, q1, m  );
  q1 = sdd_condition( -32, q1, m  );
  q1 = sdd_condition( -33, q1, m  );
  q1 = sdd_condition( -34, q1, m  );
  q1 = sdd_condition( -35, q1, m  );
  q1 = sdd_condition( -36, q1, m  );
  q1 = sdd_condition( -37, q1, m  );
  q1 = sdd_condition( -38, q1, m  );
  q1 = sdd_condition( -39, q1, m  );
  q1 = sdd_condition( -40, q1, m  );
  q1 = sdd_condition( -41, q1, m  );
  q1 = sdd_condition( -42, q1, m  );
  q1 = sdd_condition(  43, q1, m  );
  q1 = sdd_condition( -44, q1, m  );
  q1 = sdd_condition( -45, q1, m  );








	 clk_begin = clock();////// Time start
	  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



	  clk_begin = clock();////// Time start
	  SddModelCount q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////




}


void sp2_q3()
{

       	printf("\n######################################################################\n");
        printf("\n********************  SP2b Q3                                         \n");
        double time_spent=0;
        double total_time = 0.0;
        clock_t clk_begin;
        clock_t clk_end;



  SddNode* q1;

/*
  //<http://www.w3.org/1999/02/22-rdf-syntax-ns#type> is 000000000000111

  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(  -27, q1, m  );
  q1 = sdd_condition(   28, q1, m  );
  q1 = sdd_condition(   29, q1, m  );
  q1 = sdd_condition(   30, q1, m  );

*/

//binary of <http://localhost/persons/Paul_Erdoes> is 000000000000011
  
  q1 = sdd_condition( -31, kg, m  );
  q1 = sdd_condition( -32, q1, m  );
  q1 = sdd_condition( -33, q1, m  );
  q1 = sdd_condition( -34, q1, m  );
  q1 = sdd_condition( -35, q1, m  );
  q1 = sdd_condition( -36, q1, m  );
  q1 = sdd_condition( -37, q1, m  );
  q1 = sdd_condition( -38, q1, m  );
  q1 = sdd_condition( -39, q1, m  );
  q1 = sdd_condition( -40, q1, m  );
  q1 = sdd_condition( -41, q1, m  );
  q1 = sdd_condition( -42, q1, m  );
  q1 = sdd_condition( -43, q1, m  );
  q1 = sdd_condition(  44, q1, m  );
  q1 = sdd_condition(  45, q1, m  );








	 clk_begin = clock();////// Time start
	  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



	  clk_begin = clock();////// Time start
	  SddModelCount q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////




}


void sp2_q2()
{
	printf("\n######################################################################\n");
	printf("\n********************  SP2 Q2                                         \n");
	double time_spent=0;
	double total_time = 0.0;
	clock_t clk_begin;
	clock_t clk_end;


	
  SddNode* q1;
  //SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  ////////////////////////  LUBM Q1 ////////////////
  /*
  q1 = sdd_condition(  -1, q1, m  );
  q1 = sdd_condition(  -2, q1, m  );
  q1 = sdd_condition(   3, q1, m  );
  q1 = sdd_condition(   4, q1, m  );
  q1 = sdd_condition(   5, q1, m  );
  q1 = sdd_condition(  -6, q1, m  );
  q1 = sdd_condition(  -7, q1, m  );
  q1 = sdd_condition( -11, q1, m  );
  q1 = sdd_condition( -12, q1, m  );
  q1 = sdd_condition( -13, q1, m  );
  q1 = sdd_condition(  14, q1, m  );
  q1 = sdd_condition( -15, q1, m  );
  */
  //binary of <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> is 000000000000001

  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(  -27, q1, m  );
  q1 = sdd_condition(  -28, q1, m  );
  q1 = sdd_condition(  -29, q1, m  );
  q1 = sdd_condition(   30, q1, m  );


  //binary of <http://localhost/vocabulary/bench/Article> is 000000000010000
  
  
  q1 = sdd_condition( -31, q1, m  );
  q1 = sdd_condition( -32, q1, m  );
  q1 = sdd_condition( -33, q1, m  );
  q1 = sdd_condition( -34, q1, m  );
  q1 = sdd_condition( -35, q1, m  );
  q1 = sdd_condition( -36, q1, m  );
  q1 = sdd_condition( -37, q1, m  );
  q1 = sdd_condition( -38, q1, m  );
  q1 = sdd_condition( -39, q1, m  );
  q1 = sdd_condition( -40, q1, m  );
  q1 = sdd_condition(  41, q1, m  );
  q1 = sdd_condition( -42, q1, m  );
  q1 = sdd_condition( -43, q1, m  );
  q1 = sdd_condition( -44, q1, m  );
  q1 = sdd_condition( -45, q1, m  );








	 clk_begin = clock();////// Time start
	  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



	  clk_begin = clock();////// Time start
	  SddModelCount q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);


  SddNode* q2;
  //SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  ////////////////////////  LUBM Q1 ////////////////
  /*
  q1 = sdd_condition(  -1, q1, m  );
  q1 = sdd_condition(  -2, q1, m  );
  q1 = sdd_condition(   3, q1, m  );
  q1 = sdd_condition(   4, q1, m  );
  q1 = sdd_condition(   5, q1, m  );
  q1 = sdd_condition(  -6, q1, m  );
  q1 = sdd_condition(  -7, q1, m  );
  q1 = sdd_condition( -11, q1, m  );
  q1 = sdd_condition( -12, q1, m  );
  q1 = sdd_condition( -13, q1, m  );
  q1 = sdd_condition(  14, q1, m  );
  q1 = sdd_condition( -15, q1, m  );
  */
  //binary of <http://swrc.ontoware.org/ontology#isbn> is 000110110011010

  q2 = sdd_condition(  -16, kg, m  );
  q2 = sdd_condition(  -17, q2, m  );
  q2 = sdd_condition(  -18, q2, m  );
  q2 = sdd_condition(   19, q2, m  );
  q2 = sdd_condition(   20, q2, m  );
  q2 = sdd_condition(  -21, q2, m  );
  q2 = sdd_condition(   22, q2, m  );
  q2 = sdd_condition(   23, q2, m  );
  q2 = sdd_condition(  -24, q2, m  );
  q2 = sdd_condition(  -25, q2, m  );
  q2 = sdd_condition(   26, q2, m  );
  q2 = sdd_condition(   27, q2, m  );
  q2 = sdd_condition(  -28, q2, m  );
  q2 = sdd_condition(   29, q2, m  );
  q2 = sdd_condition(  -30, q2, m  );


  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##Department> is 000110001111000

/*
  q2 = sdd_condition( -31, q2, m  );
  q2 = sdd_condition( -32, q2, m  );
  q2 = sdd_condition( -33, q2, m  );
  q2 = sdd_condition(  34, q2, m  );
  q2 = sdd_condition(  35, q2, m  );
  q2 = sdd_condition( -36, q2, m  );
  q2 = sdd_condition( -37, q2, m  );
  q2 = sdd_condition( -38, q2, m  );
  q2 = sdd_condition(  39, q2, m  );
  q2 = sdd_condition(  40, q2, m  );
  q2 = sdd_condition(  41, q2, m  );
  q2 = sdd_condition(  42, q2, m  );
  q2 = sdd_condition( -43, q2, m  );
  q2 = sdd_condition( -44, q2, m  );
  q2 = sdd_condition( -45, q2, m  );
*/







         clk_begin = clock();////// Time start
          printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q2) ? "F":"T");
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



          clk_begin = clock();////// Time start
          q1_count  = sdd_model_count(q2,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);



  /*
   * Join_1 (SS)
   *
   */

          clk_begin = clock();////// Time start
	  join_sdd2(q1,q2,1,15,1,15); //latest
  	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for join q1 & q2 (ss) \n", time_spent);//Print!!!!
	  total_time += time_spent;









/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
}




void sp2_q1()
{
	printf("\n######################################################################\n");
	printf("\n********************  SP2 Q1                                         \n");
	double time_spent=0;
	double total_time = 0.0;
	clock_t clk_begin;
	clock_t clk_end;


	
  SddNode* q1;
  //SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  ////////////////////////  LUBM Q1 ////////////////
  /*
  q1 = sdd_condition(  -1, q1, m  );
  q1 = sdd_condition(  -2, q1, m  );
  q1 = sdd_condition(   3, q1, m  );
  q1 = sdd_condition(   4, q1, m  );
  q1 = sdd_condition(   5, q1, m  );
  q1 = sdd_condition(  -6, q1, m  );
  q1 = sdd_condition(  -7, q1, m  );
  q1 = sdd_condition( -11, q1, m  );
  q1 = sdd_condition( -12, q1, m  );
  q1 = sdd_condition( -13, q1, m  );
  q1 = sdd_condition(  14, q1, m  );
  q1 = sdd_condition( -15, q1, m  );
  */
  //<http://purl.org/dc/elements/1.1/title> is 000000000001010

  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(   27, q1, m  );
  q1 = sdd_condition(  -28, q1, m  );
  q1 = sdd_condition(   29, q1, m  );
  q1 = sdd_condition(  -30, q1, m  );


  //binary of "Journal 1 (1940)"^^<http://www.w3.org/2001/XMLSchema#string> is 100101111100111
  
  
  q1 = sdd_condition(  31, q1, m  );
  q1 = sdd_condition( -32, q1, m  );
  q1 = sdd_condition( -33, q1, m  );
  q1 = sdd_condition(  34, q1, m  );
  q1 = sdd_condition( -35, q1, m  );
  q1 = sdd_condition(  36, q1, m  );
  q1 = sdd_condition(  37, q1, m  );
  q1 = sdd_condition(  38, q1, m  );
  q1 = sdd_condition(  39, q1, m  );
  q1 = sdd_condition(  40, q1, m  );
  q1 = sdd_condition( -41, q1, m  );
  q1 = sdd_condition( -42, q1, m  );
  q1 = sdd_condition(  43, q1, m  );
  q1 = sdd_condition(  44, q1, m  );
  q1 = sdd_condition(  45, q1, m  );








	 clk_begin = clock();////// Time start
	  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



	  clk_begin = clock();////// Time start
	  SddModelCount q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);


  SddNode* q2;
  //SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  ////////////////////////  LUBM Q1 ////////////////
  /*
  q1 = sdd_condition(  -1, q1, m  );
  q1 = sdd_condition(  -2, q1, m  );
  q1 = sdd_condition(   3, q1, m  );
  q1 = sdd_condition(   4, q1, m  );
  q1 = sdd_condition(   5, q1, m  );
  q1 = sdd_condition(  -6, q1, m  );
  q1 = sdd_condition(  -7, q1, m  );
  q1 = sdd_condition( -11, q1, m  );
  q1 = sdd_condition( -12, q1, m  );
  q1 = sdd_condition( -13, q1, m  );
  q1 = sdd_condition(  14, q1, m  );
  q1 = sdd_condition( -15, q1, m  );
  */
  //binary of <http://purl.org/dc/terms/issued> is 000110110010110

  q2 = sdd_condition(  -16, kg, m  );
  q2 = sdd_condition(  -17, q2, m  );
  q2 = sdd_condition(  -18, q2, m  );
  q2 = sdd_condition(   19, q2, m  );
  q2 = sdd_condition(   20, q2, m  );
  q2 = sdd_condition(  -21, q2, m  );
  q2 = sdd_condition(   22, q2, m  );
  q2 = sdd_condition(   23, q2, m  );
  q2 = sdd_condition(  -24, q2, m  );
  q2 = sdd_condition(  -25, q2, m  );
  q2 = sdd_condition(   26, q2, m  );
  q2 = sdd_condition(  -27, q2, m  );
  q2 = sdd_condition(   28, q2, m  );
  q2 = sdd_condition(   29, q2, m  );
  q2 = sdd_condition(  -30, q2, m  );


  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##Department> is 000110001111000

/*
  q2 = sdd_condition( -31, q2, m  );
  q2 = sdd_condition( -32, q2, m  );
  q2 = sdd_condition( -33, q2, m  );
  q2 = sdd_condition(  34, q2, m  );
  q2 = sdd_condition(  35, q2, m  );
  q2 = sdd_condition( -36, q2, m  );
  q2 = sdd_condition( -37, q2, m  );
  q2 = sdd_condition( -38, q2, m  );
  q2 = sdd_condition(  39, q2, m  );
  q2 = sdd_condition(  40, q2, m  );
  q2 = sdd_condition(  41, q2, m  );
  q2 = sdd_condition(  42, q2, m  );
  q2 = sdd_condition( -43, q2, m  );
  q2 = sdd_condition( -44, q2, m  );
  q2 = sdd_condition( -45, q2, m  );
*/







         clk_begin = clock();////// Time start
          printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q2) ? "F":"T");
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



          clk_begin = clock();////// Time start
          q1_count  = sdd_model_count(q2,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);



  /*
   * Join_1 (SS)
   *
   */

          clk_begin = clock();////// Time start
	  join_sdd2(q1,q2,1,15,1,15); //latest
  	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for join q1 & q2 (ss) \n", time_spent);//Print!!!!
	  total_time += time_spent;









/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
}






























































void lubm_q7()
{
	printf("\n######################################################################\n");
	printf("\n********************  LUBM Q7                                         \n");
	double time_spent=0;
	double total_time = 0.0;
	clock_t clk_begin;
	clock_t clk_end;


	
  SddNode* q1;

  //<http://www.w3.org/1999/02/22-rdf-syntax-ns#type> is 000000000000111

  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(  -27, q1, m  );
  q1 = sdd_condition(   28, q1, m  );
  q1 = sdd_condition(   29, q1, m  );
  q1 = sdd_condition(   30, q1, m  );


  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##FullProfessor> is 000001011101101


  q1 = sdd_condition( -31, q1, m  );
  q1 = sdd_condition( -32, q1, m  );
  q1 = sdd_condition( -33, q1, m  );
  q1 = sdd_condition( -34, q1, m  );
  q1 = sdd_condition( -35, q1, m  );
  q1 = sdd_condition(  36, q1, m  );
  q1 = sdd_condition( -37, q1, m  );
  q1 = sdd_condition(  38, q1, m  );
  q1 = sdd_condition(  39, q1, m  );
  q1 = sdd_condition(  40, q1, m  );
  q1 = sdd_condition( -41, q1, m  );
  q1 = sdd_condition(  42, q1, m  );
  q1 = sdd_condition(  43, q1, m  );
  q1 = sdd_condition( -44, q1, m  );
  q1 = sdd_condition(  45, q1, m  );








	 clk_begin = clock();////// Time start
	  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



	  clk_begin = clock();////// Time start
	  SddModelCount q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);


  SddNode* q2;
  // <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##advisor> is 000010000111110

  q2 = sdd_condition(  -16, kg, m  );
  q2 = sdd_condition(  -17, q2, m  );
  q2 = sdd_condition(  -18, q2, m  );
  q2 = sdd_condition(  -19, q2, m  );
  q2 = sdd_condition(   20, q2, m  );
  q2 = sdd_condition(  -21, q2, m  );
  q2 = sdd_condition(  -22, q2, m  );
  q2 = sdd_condition(  -23, q2, m  );
  q2 = sdd_condition(  -24, q2, m  );
  q2 = sdd_condition(   25, q2, m  );
  q2 = sdd_condition(   26, q2, m  );
  q2 = sdd_condition(   27, q2, m  );
  q2 = sdd_condition(   28, q2, m  );
  q2 = sdd_condition(   29, q2, m  );
  q2 = sdd_condition(  -30, q2, m  );








         clk_begin = clock();////// Time start
          printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q2) ? "F":"T");
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



          clk_begin = clock();////// Time start
          q1_count  = sdd_model_count(q2,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);



  /*
   * Join_1 (SS)
   *
   */

          clk_begin = clock();////// Time start
	  join_sdd2(q1,q2,1,15,31,45); //latest
  	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for join q1 & q2 (ss) \n", time_spent);//Print!!!!
	  total_time += time_spent;



//<http://www.w3.org/1999/02/22-rdf-syntax-ns#type> is 000000000000111

  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(  -27, q1, m  );
  q1 = sdd_condition(   28, q1, m  );
  q1 = sdd_condition(   29, q1, m  );
  q1 = sdd_condition(   30, q1, m  );


  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##UndergraduateStudent> is 000011000111101

  q1 = sdd_condition( -31, q1, m  );
  q1 = sdd_condition( -32, q1, m  );
  q1 = sdd_condition( -33, q1, m  );
  q1 = sdd_condition( -34, q1, m  );
  q1 = sdd_condition(  35, q1, m  );
  q1 = sdd_condition(  36, q1, m  );
  q1 = sdd_condition( -37, q1, m  );
  q1 = sdd_condition( -38, q1, m  );
  q1 = sdd_condition( -39, q1, m  );
  q1 = sdd_condition(  40, q1, m  );
  q1 = sdd_condition(  41, q1, m  );
  q1 = sdd_condition(  42, q1, m  );
  q1 = sdd_condition(  43, q1, m  );
  q1 = sdd_condition( -44, q1, m  );
  q1 = sdd_condition(  45, q1, m  );






         clk_begin = clock();////// Time start
          printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



          clk_begin = clock();////// Time start
          q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);









  /*
   * Join_2 (SS)
   *
   */




  
  
  q2 = _intersection_sdd;

          clk_begin = clock();////// Time start
          join_sdd2(q2,q1,1,15,1,15); //latest
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
	  total_time += time_spent;
          printf("Time elpased is %f seconds for final join  \n total:%f\n\n", time_spent,total_time);//Print!!!!



	  

	  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##takesCourse> is 000010001000010

	  

  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(   20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(   24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(  -27, q1, m  );
  q1 = sdd_condition(  -28, q1, m  );
  q1 = sdd_condition(   29, q1, m  );
  q1 = sdd_condition(  -30, q1, m  );







         clk_begin = clock();////// Time start
          printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
          total_time += time_spent;



          clk_begin = clock();////// Time start
          q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);









  /*
   * Join_2 (SS)
   *
   */






  q2 = _intersection_sdd;

          clk_begin = clock();////// Time start
          join_sdd2(q2,q1,1,15,1,15); //latest
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          total_time += time_spent;
          printf("Time elpased is %f seconds for final join  \n total:%f\n\n", time_spent,total_time);//Print!!!!








/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
}








void lubm_q6()
{
	printf("\n######################################################################\n");
	printf("\n********************  LUBM Q6                                         \n");
	double time_spent=0;
	double total_time = 0.0;
	clock_t clk_begin;
	clock_t clk_end;


	
  SddNode* q1;
  //SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  ////////////////////////  LUBM Q1 ////////////////
  ////<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##worksFor> is 000000000101001


  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(   25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(   27, q1, m  );
  q1 = sdd_condition(  -28, q1, m  );
  q1 = sdd_condition(  -29, q1, m  );
  q1 = sdd_condition(   30, q1, m  );





	 clk_begin = clock();////// Time start
	  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



	  clk_begin = clock();////// Time start
	  SddModelCount q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);


  SddNode* q2;
  //SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  ////////////////////////  LUBM Q1 ////////////////
  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##subOrganizationOf> is 000011000101111

  q2 = sdd_condition(  -16, kg, m  );
  q2 = sdd_condition(  -17, q2, m  );
  q2 = sdd_condition(  -18, q2, m  );
  q2 = sdd_condition(  -19, q2, m  );
  q2 = sdd_condition(   20, q2, m  );
  q2 = sdd_condition(   21, q2, m  );
  q2 = sdd_condition(  -22, q2, m  );
  q2 = sdd_condition(  -23, q2, m  );
  q2 = sdd_condition(  -24, q2, m  );
  q2 = sdd_condition(   25, q2, m  );
  q2 = sdd_condition(  -26, q2, m  );
  q2 = sdd_condition(   27, q2, m  );
  q2 = sdd_condition(   28, q2, m  );
  q2 = sdd_condition(   29, q2, m  );
  q2 = sdd_condition(   30, q2, m  );


  //binary of <http://www.University0.edu> is 000000001011011

  q2 = sdd_condition( -31, q2, m  );
  q2 = sdd_condition( -32, q2, m  );
  q2 = sdd_condition( -33, q2, m  );
  q2 = sdd_condition( -34, q2, m  );
  q2 = sdd_condition( -35, q2, m  );
  q2 = sdd_condition( -36, q2, m  );
  q2 = sdd_condition( -37, q2, m  );
  q2 = sdd_condition( -38, q2, m  );
  q2 = sdd_condition(  39, q2, m  );
  q2 = sdd_condition( -40, q2, m  );
  q2 = sdd_condition(  41, q2, m  );
  q2 = sdd_condition(  42, q2, m  );
  q2 = sdd_condition( -43, q2, m  );
  q2 = sdd_condition(  44, q2, m  );
  q2 = sdd_condition(  45, q2, m  );








         clk_begin = clock();////// Time start
          printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q2) ? "F":"T");
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



          clk_begin = clock();////// Time start
          q1_count  = sdd_model_count(q2,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);



  /*
   * Join_1 (SS)
   *
   */

          clk_begin = clock();////// Time start
	  join_sdd2(q2,q1,1,15,31,45); //latest
  	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for join q1 & q2 (ss) \n", time_spent);//Print!!!!
	  total_time += time_spent;


 //<http://www.w3.org/1999/02/22-rdf-syntax-ns#type> is 000000000000111

  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(  -27, q1, m  );
  q1 = sdd_condition(   28, q1, m  );
  q1 = sdd_condition(   29, q1, m  );
  q1 = sdd_condition(   30, q1, m  );


  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##FullProfessor> is 000001011101101


  q1 = sdd_condition( -31, q1, m  );
  q1 = sdd_condition( -32, q1, m  );
  q1 = sdd_condition( -33, q1, m  );
  q1 = sdd_condition( -34, q1, m  );
  q1 = sdd_condition( -35, q1, m  );
  q1 = sdd_condition(  36, q1, m  );
  q1 = sdd_condition( -37, q1, m  );
  q1 = sdd_condition(  38, q1, m  );
  q1 = sdd_condition(  39, q1, m  );
  q1 = sdd_condition(  40, q1, m  );
  q1 = sdd_condition( -41, q1, m  );
  q1 = sdd_condition(  42, q1, m  );
  q1 = sdd_condition(  43, q1, m  );
  q1 = sdd_condition( -44, q1, m  );
  q1 = sdd_condition(  45, q1, m  );








         clk_begin = clock();////// Time start
          printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



          clk_begin = clock();////// Time start
          q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);









  /*
   * Join_2 (SS)
   *
   */




  
  
  q2 = _intersection_sdd;

          clk_begin = clock();////// Time start
          join_sdd2(q1,q2,1,15,1,15); //latest
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
	  total_time += time_spent;
          printf("Time elpased is %f seconds for final join  \n total:%f\n\n", time_spent,total_time);//Print!!!!







/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
}























void lubm_q5()
{

       	printf("\n######################################################################\n");
        printf("\n********************  LUBM Q5                                         \n");
        double time_spent=0;
        double total_time = 0.0;
        clock_t clk_begin;
        clock_t clk_end;


  SddNode* q2;

  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##subOrganizationOf> is 000011000101111

  q2 = sdd_condition(  -16, kg, m  );
  q2 = sdd_condition(  -17, q2, m  );
  q2 = sdd_condition(  -18, q2, m  );
  q2 = sdd_condition(  -19, q2, m  );
  q2 = sdd_condition(   20, q2, m  );
  q2 = sdd_condition(   21, q2, m  );
  q2 = sdd_condition(  -22, q2, m  );
  q2 = sdd_condition(  -23, q2, m  );
  q2 = sdd_condition(  -24, q2, m  );
  q2 = sdd_condition(   25, q2, m  );
  q2 = sdd_condition(  -26, q2, m  );
  q2 = sdd_condition(   27, q2, m  );
  q2 = sdd_condition(   28, q2, m  );
  q2 = sdd_condition(   29, q2, m  );
  q2 = sdd_condition(   30, q2, m  );

  //<http://www.Department0.University0.edu> is 000000000101010


  q2 = sdd_condition( -31, q2, m  );
  q2 = sdd_condition( -32, q2, m  );
  q2 = sdd_condition( -33, q2, m  );
  q2 = sdd_condition( -34, q2, m  );
  q2 = sdd_condition( -35, q2, m  );
  q2 = sdd_condition( -36, q2, m  );
  q2 = sdd_condition( -37, q2, m  );
  q2 = sdd_condition( -38, q2, m  );
  q2 = sdd_condition( -39, q2, m  );
  q2 = sdd_condition(  40, q2, m  );
  q2 = sdd_condition( -41, q2, m  );
  q2 = sdd_condition(  42, q2, m  );
  q2 = sdd_condition( -43, q2, m  );
  q2 = sdd_condition(  44, q2, m  );
  q2 = sdd_condition( -45, q2, m  );




	  clk_begin = clock();////// Time start
	  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q2) ? "F":"T");
	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



	  clk_begin = clock();////// Time start
	  SddModelCount q1_count  = sdd_model_count(q2,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);

}




void lubm_q4()
{

       	printf("\n######################################################################\n");
        printf("\n********************  LUBM Q4                                         \n");
        double time_spent=0;
        double total_time = 0.0;
        clock_t clk_begin;
        clock_t clk_end;


  SddNode* q1;
  //SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  ////////////////////////  LUBM Q1 ////////////////
  /*
  q1 = sdd_condition(  -1, q1, m  );
  q1 = sdd_condition(  -2, q1, m  );
  q1 = sdd_condition(   3, q1, m  );
  q1 = sdd_condition(   4, q1, m  );
  q1 = sdd_condition(   5, q1, m  );
  q1 = sdd_condition(  -6, q1, m  );
  q1 = sdd_condition(  -7, q1, m  );
  q1 = sdd_condition( -11, q1, m  );
  q1 = sdd_condition( -12, q1, m  );
  q1 = sdd_condition( -13, q1, m  );
  q1 = sdd_condition(  14, q1, m  );
  q1 = sdd_condition( -15, q1, m  );
  */
  //<http://www.w3.org/1999/02/22-rdf-syntax-ns#type> is 000000000000111

  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(  -27, q1, m  );
  q1 = sdd_condition(   28, q1, m  );
  q1 = sdd_condition(   29, q1, m  );
  q1 = sdd_condition(   30, q1, m  );


  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##FullProfessor> is 000001011101101

  
  q1 = sdd_condition( -31, q1, m  );
  q1 = sdd_condition( -32, q1, m  );
  q1 = sdd_condition( -33, q1, m  );
  q1 = sdd_condition( -34, q1, m  );
  q1 = sdd_condition( -35, q1, m  );
  q1 = sdd_condition(  36, q1, m  );
  q1 = sdd_condition( -37, q1, m  );
  q1 = sdd_condition(  38, q1, m  );
  q1 = sdd_condition(  39, q1, m  );
  q1 = sdd_condition(  40, q1, m  );
  q1 = sdd_condition( -41, q1, m  );
  q1 = sdd_condition(  42, q1, m  );
  q1 = sdd_condition(  43, q1, m  );
  q1 = sdd_condition( -44, q1, m  );
  q1 = sdd_condition(  45, q1, m  );








	 clk_begin = clock();////// Time start
	  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



	  clk_begin = clock();////// Time start
	  SddModelCount q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);


  SddNode* q2;
  //SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  ////////////////////////  LUBM Q1 ////////////////
  /*
  q1 = sdd_condition(  -1, q1, m  );
  q1 = sdd_condition(  -2, q1, m  );
  q1 = sdd_condition(   3, q1, m  );
  q1 = sdd_condition(   4, q1, m  );
  q1 = sdd_condition(   5, q1, m  );
  q1 = sdd_condition(  -6, q1, m  );
  q1 = sdd_condition(  -7, q1, m  );
  q1 = sdd_condition( -11, q1, m  );
  q1 = sdd_condition( -12, q1, m  );
  q1 = sdd_condition( -13, q1, m  );
  q1 = sdd_condition(  14, q1, m  );
  q1 = sdd_condition( -15, q1, m  );
  */
  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##worksFor> is 000000000101001

  q2 = sdd_condition(  -16, kg, m  );
  q2 = sdd_condition(  -17, q2, m  );
  q2 = sdd_condition(  -18, q2, m  );
  q2 = sdd_condition(  -19, q2, m  );
  q2 = sdd_condition(  -20, q2, m  );
  q2 = sdd_condition(  -21, q2, m  );
  q2 = sdd_condition(  -22, q2, m  );
  q2 = sdd_condition(  -23, q2, m  );
  q2 = sdd_condition(  -24, q2, m  );
  q2 = sdd_condition(   25, q2, m  );
  q2 = sdd_condition(  -26, q2, m  );
  q2 = sdd_condition(   27, q2, m  );
  q2 = sdd_condition(  -28, q2, m  );
  q2 = sdd_condition(  -29, q2, m  );
  q2 = sdd_condition(   30, q2, m  );


  //<http://www.Department0.University0.edu> is 000000000101010


  q2 = sdd_condition( -31, q2, m  );
  q2 = sdd_condition( -32, q2, m  );
  q2 = sdd_condition( -33, q2, m  );
  q2 = sdd_condition( -34, q2, m  );
  q2 = sdd_condition( -35, q2, m  );
  q2 = sdd_condition( -36, q2, m  );
  q2 = sdd_condition( -37, q2, m  );
  q2 = sdd_condition( -38, q2, m  );
  q2 = sdd_condition( -39, q2, m  );
  q2 = sdd_condition(  40, q2, m  );
  q2 = sdd_condition( -41, q2, m  );
  q2 = sdd_condition(  42, q2, m  );
  q2 = sdd_condition( -43, q2, m  );
  q2 = sdd_condition(  44, q2, m  );
  q2 = sdd_condition( -45, q2, m  );








         clk_begin = clock();////// Time start
          printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q2) ? "F":"T");
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



          clk_begin = clock();////// Time start
          q1_count  = sdd_model_count(q2,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);



  /*
   * Join_1 (SS)
   *
   */

          clk_begin = clock();////// Time start
	  join_sdd2(q1,q2,1,15,1,15); //latest
  	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for join q1 & q2 (ss) \n", time_spent);//Print!!!!
	  total_time += time_spent;







}



void lubm_q3()
{

       	printf("\n######################################################################\n");
        printf("\n********************  LUBM Q3                                         \n");
	lubm_q1();

}


void lubm_q2()
{

       	printf("\n######################################################################\n");
        printf("\n********************  LUBM Q2                                         \n");
        double time_spent=0;
        double total_time = 0.0;
        clock_t clk_begin;
        clock_t clk_end;



  SddNode* q1;
  //<http://www.w3.org/1999/02/22-rdf-syntax-ns#type> is 000000000000111

  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(  -27, q1, m  );
  q1 = sdd_condition(   28, q1, m  );
  q1 = sdd_condition(   29, q1, m  );
  q1 = sdd_condition(   30, q1, m  );


//<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##Course> is 000001010000000
  
  q1 = sdd_condition( -31, q1, m  );
  q1 = sdd_condition( -32, q1, m  );
  q1 = sdd_condition( -33, q1, m  );
  q1 = sdd_condition( -34, q1, m  );
  q1 = sdd_condition( -35, q1, m  );
  q1 = sdd_condition(  36, q1, m  );
  q1 = sdd_condition( -37, q1, m  );
  q1 = sdd_condition(  38, q1, m  );
  q1 = sdd_condition( -39, q1, m  );
  q1 = sdd_condition( -40, q1, m  );
  q1 = sdd_condition( -41, q1, m  );
  q1 = sdd_condition( -42, q1, m  );
  q1 = sdd_condition( -43, q1, m  );
  q1 = sdd_condition( -44, q1, m  );
  q1 = sdd_condition( -45, q1, m  );








	 clk_begin = clock();////// Time start
	  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



	  clk_begin = clock();////// Time start
	  SddModelCount q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////




}




void lubm_q1()
{
	printf("\n######################################################################\n");
	printf("\n********************  LUBM Q1                                         \n");
	double time_spent=0;
	double total_time = 0.0;
	clock_t clk_begin;
	clock_t clk_end;


	
  SddNode* q1;
  //SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  ////////////////////////  LUBM Q1 ////////////////
  /*
  q1 = sdd_condition(  -1, q1, m  );
  q1 = sdd_condition(  -2, q1, m  );
  q1 = sdd_condition(   3, q1, m  );
  q1 = sdd_condition(   4, q1, m  );
  q1 = sdd_condition(   5, q1, m  );
  q1 = sdd_condition(  -6, q1, m  );
  q1 = sdd_condition(  -7, q1, m  );
  q1 = sdd_condition( -11, q1, m  );
  q1 = sdd_condition( -12, q1, m  );
  q1 = sdd_condition( -13, q1, m  );
  q1 = sdd_condition(  14, q1, m  );
  q1 = sdd_condition( -15, q1, m  );
  */
  //<http://www.w3.org/1999/02/22-rdf-syntax-ns#type> is 000000000000111

  q1 = sdd_condition(  -16, kg, m  );
  q1 = sdd_condition(  -17, q1, m  );
  q1 = sdd_condition(  -18, q1, m  );
  q1 = sdd_condition(  -19, q1, m  );
  q1 = sdd_condition(  -20, q1, m  );
  q1 = sdd_condition(  -21, q1, m  );
  q1 = sdd_condition(  -22, q1, m  );
  q1 = sdd_condition(  -23, q1, m  );
  q1 = sdd_condition(  -24, q1, m  );
  q1 = sdd_condition(  -25, q1, m  );
  q1 = sdd_condition(  -26, q1, m  );
  q1 = sdd_condition(  -27, q1, m  );
  q1 = sdd_condition(   28, q1, m  );
  q1 = sdd_condition(   29, q1, m  );
  q1 = sdd_condition(   30, q1, m  );


  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##Department> is 000110001111000

  
  q1 = sdd_condition( -31, q1, m  );
  q1 = sdd_condition( -32, q1, m  );
  q1 = sdd_condition( -33, q1, m  );
  q1 = sdd_condition(  34, q1, m  );
  q1 = sdd_condition(  35, q1, m  );
  q1 = sdd_condition( -36, q1, m  );
  q1 = sdd_condition( -37, q1, m  );
  q1 = sdd_condition( -38, q1, m  );
  q1 = sdd_condition(  39, q1, m  );
  q1 = sdd_condition(  40, q1, m  );
  q1 = sdd_condition(  41, q1, m  );
  q1 = sdd_condition(  42, q1, m  );
  q1 = sdd_condition( -43, q1, m  );
  q1 = sdd_condition( -44, q1, m  );
  q1 = sdd_condition( -45, q1, m  );








	 clk_begin = clock();////// Time start
	  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q1) ? "F":"T");
	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



	  clk_begin = clock();////// Time start
	  SddModelCount q1_count  = sdd_model_count(q1,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);


  SddNode* q2;
  //SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  ////////////////////////  LUBM Q1 ////////////////
  /*
  q1 = sdd_condition(  -1, q1, m  );
  q1 = sdd_condition(  -2, q1, m  );
  q1 = sdd_condition(   3, q1, m  );
  q1 = sdd_condition(   4, q1, m  );
  q1 = sdd_condition(   5, q1, m  );
  q1 = sdd_condition(  -6, q1, m  );
  q1 = sdd_condition(  -7, q1, m  );
  q1 = sdd_condition( -11, q1, m  );
  q1 = sdd_condition( -12, q1, m  );
  q1 = sdd_condition( -13, q1, m  );
  q1 = sdd_condition(  14, q1, m  );
  q1 = sdd_condition( -15, q1, m  );
  */
  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##subOrganizationOf> is 000011000101111

  q2 = sdd_condition(  -16, kg, m  );
  q2 = sdd_condition(  -17, q2, m  );
  q2 = sdd_condition(  -18, q2, m  );
  q2 = sdd_condition(  -19, q2, m  );
  q2 = sdd_condition(   20, q2, m  );
  q2 = sdd_condition(   21, q2, m  );
  q2 = sdd_condition(  -22, q2, m  );
  q2 = sdd_condition(  -23, q2, m  );
  q2 = sdd_condition(  -24, q2, m  );
  q2 = sdd_condition(   25, q2, m  );
  q2 = sdd_condition(  -26, q2, m  );
  q2 = sdd_condition(   27, q2, m  );
  q2 = sdd_condition(   28, q2, m  );
  q2 = sdd_condition(   29, q2, m  );
  q2 = sdd_condition(   30, q2, m  );


  //<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##Department> is 000110001111000

/*
  q2 = sdd_condition( -31, q2, m  );
  q2 = sdd_condition( -32, q2, m  );
  q2 = sdd_condition( -33, q2, m  );
  q2 = sdd_condition(  34, q2, m  );
  q2 = sdd_condition(  35, q2, m  );
  q2 = sdd_condition( -36, q2, m  );
  q2 = sdd_condition( -37, q2, m  );
  q2 = sdd_condition( -38, q2, m  );
  q2 = sdd_condition(  39, q2, m  );
  q2 = sdd_condition(  40, q2, m  );
  q2 = sdd_condition(  41, q2, m  );
  q2 = sdd_condition(  42, q2, m  );
  q2 = sdd_condition( -43, q2, m  );
  q2 = sdd_condition( -44, q2, m  );
  q2 = sdd_condition( -45, q2, m  );
*/







         clk_begin = clock();////// Time start
          printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q2) ? "F":"T");
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



          clk_begin = clock();////// Time start
          q1_count  = sdd_model_count(q2,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);



  /*
   * Join_1 (SS)
   *
   */

          clk_begin = clock();////// Time start
	  join_sdd2(q1,q2,1,15,1,15); //latest
  	  clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for join q1 & q2 (ss) \n", time_spent);//Print!!!!
	  total_time += time_spent;




 // <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl##undergraduateDegreeFrom> is 000000000100111


  q2 = sdd_condition(  -16, kg, m  );
  q2 = sdd_condition(  -17, q2, m  );
  q2 = sdd_condition(  -18, q2, m  );
  q2 = sdd_condition(  -19, q2, m  );
  q2 = sdd_condition(  -20, q2, m  );
  q2 = sdd_condition(  -21, q2, m  );
  q2 = sdd_condition(  -22, q2, m  );
  q2 = sdd_condition(  -23, q2, m  );
  q2 = sdd_condition(  -24, q2, m  );
  q2 = sdd_condition(   25, q2, m  );
  q2 = sdd_condition(  -26, q2, m  );
  q2 = sdd_condition(  -27, q2, m  );
  q2 = sdd_condition(   28, q2, m  );
  q2 = sdd_condition(   29, q2, m  );
  q2 = sdd_condition(   30, q2, m  );







         clk_begin = clock();////// Time start
          printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(q2) ? "F":"T");
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for node is false (SAT) \n", time_spent);//Print!!!!
	  total_time += time_spent;



          clk_begin = clock();////// Time start
          q1_count  = sdd_model_count(q2,m);
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
          printf("Time elpased is %f seconds for model count \n", time_spent);//Print!!!!

  //SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",q1_count);
  //printf("\n sdd global model count       : %"PRImcS"\n",gcount);









  /*
   * Join_2 (SS)
   *
   */




  
  
  q1 = _intersection_sdd;

          clk_begin = clock();////// Time start
          join_sdd2(q1,q2,31,45,31,45); //latest
          clk_end = clock();//////// Time end
          time_spent = (double)(clk_end - clk_begin) / CLOCKS_PER_SEC;///// Elapsed
	  total_time += time_spent;
          printf("Time elpased is %f seconds for final join  \n total:%f\n\n", time_spent,total_time);//Print!!!!







/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
}














////////////////////////  END of FIL!!!!!!


  /*


  SddNode* q1 = kg;
  SddNode* q2 = kg;
  //SddNode* q3 = sdd_manager_false(m);

  q1 = sdd_condition(-1, q1, m  );
  q1 = sdd_condition(-2, q1, m  );
  q1 = sdd_condition(3, q1, m  );
  q1 = sdd_condition(4, q1, m  );
  q1 = sdd_condition(5, q1, m  );
  q1 = sdd_condition(-6, q1, m  );
  q1 = sdd_condition(-7, q1, m  );

 // <http://data.semanticweb.org/ns/swc/ontology#isPartOf> is 0001011110
 
  q1 = sdd_condition( -11, q1, m  );
  q1 = sdd_condition( -12, q1, m  );
  q1 = sdd_condition( -13, q1, m  );
  q1 = sdd_condition(  14, q1, m  );
  q1 = sdd_condition( -15, q1, m  );
  q1 = sdd_condition(  16, q1, m  );
  q1 = sdd_condition(  17, q1, m  );
  q1 = sdd_condition(  18, q1, m  );
  q1 = sdd_condition(  19, q1, m  );
  q1 = sdd_condition( -20, q1, m  );

  



  
  q2 = sdd_condition(1, q2, m  );
  q2 = sdd_condition(-2, q2, m  );
  q2 = sdd_condition(-3, q2, m  );
  q2 = sdd_condition(-4, q2, m  );
  q2 = sdd_condition(-5, q2, m  );
  q2 = sdd_condition(6, q2, m  );
  q2 = sdd_condition(7, q2, m  );
  
 // <http://data.semanticweb.org/ns/swc/ontology#hasPart> is 0 0 1 0 1 0 1 0 1 0  


  q2 = sdd_condition( -11, q2, m  );
  q2 = sdd_condition( -12, q2, m  );
  q2 = sdd_condition(  13, q2, m  );
  q2 = sdd_condition( -14, q2, m  );
  q2 = sdd_condition(  15, q2, m  );
  q2 = sdd_condition(  -16, q2, m  );
  q2 = sdd_condition( 17, q2, m  );
  q2 = sdd_condition(  -18, q2, m  );
  q2 = sdd_condition(  19, q2, m  );
  q2 = sdd_condition( -20, q2, m  );

  

  kg = sdd_condition(14, kg, m  );
  kg = sdd_condition(15, kg, m  );
  
  kg = sdd_condition(-16, kg, m  );
  kg = sdd_condition(-17, kg, m  );
  kg = sdd_condition(18, kg, m  );
  kg = sdd_condition(-19, kg, m  );
  kg = sdd_condition(-20, kg, m  );
  kg = sdd_condition(21, kg, m  );
  kg = sdd_condition(22, kg, m  );
  kg = sdd_condition(23, kg, m  );
  */
/*
  int* sdd_v = sdd_variables(q1, m);

  for (size_t i=1; i<24; i++ )
	  printf("\n var %d: %d", i,sdd_v[i]);

  printf("\n ################################# \n");


  //q3 = sdd_disjoin(q3,q2,m);
  //q3 = sdd_disjoin(q3,q1,m);


  
  int* sdd_v2 = sdd_variables(q2, m);

  for (size_t i=1; i<41; i++ )
          printf("\nB var %d: %d", i, sdd_v2[i]);

  
  printf("\n ################################# \n");

  //join_sdd(q1,q2,1,bit_subject_size+1);



         join_sdd2(q1,q2,1,10,21,30); //latest
	 decompile__print__triples (q2, 31, 40);


  printf("\n ################################# \n");










  //sdd_split( 21, 30, kg);
  //sdd_match(1, 10, kg);

 // q3 = sdd_disjoin(q3,q2,m);
 // q3 = sdd_disjoin(q3,q2,m);
 
#if (DEBUG==3)
  {
  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(kg) ? "F":"T");
  SddModelCount count  = sdd_model_count(kg,m);
  SddModelCount gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",count);
  printf("\n sdd global model count       : %"PRImcS"\n",gcount);

  sdd_ref(kg,m);
  //sdd_manager_minimize(m);

  printf("\n\n observation?  : %s\n\n\n", sdd_node_is_false(kg) ? "F":"T");
  count  = sdd_model_count(kg,m);
  gcount = sdd_global_model_count(kg,m);
  printf("\n sdd query model count       : %"PRImcS"\n",count);
  printf("\n sdd global model count       : %"PRImcS"\n",gcount);

  }
 #endif



*/
        




  /* 
  for (size_t i=1; i< (number_of_vars) ; i++)
  {
	  if (i != 14 && i != 15)
		  delta = sdd_condition(i,delta,m);
  }
	 // delta = sdd_condition(15,delta,m);
	 // delta = sdd_condition(16,delta,m);
 

  int is_abnormal = delta == sdd_node_is_true(delta); //sdd_manager_false(m);
  printf("\n observations normal?  : %s\n", is_abnormal ? "abnormal":"normal");
  
  printf("  sdd size = %zu\n", sdd_manager_size(delta));
  printf("  sdd count = %zu\n", sdd_manager_count(delta));
  printf("  live sdd size = %zu\n", sdd_manager_live_size(delta));
  printf("  dead sdd size = %zu\n", sdd_manager_dead_size(delta));
  sdd_manager_print(m);
  printf("sdd var count       : %"PRIlitS"\n",sdd_manager_var_count(delta));
  printf("sdd min cardinality       : %"PRIlitS"\n",sdd_minimum_cardinality(delta));
  */
  



 // g_hash_table_destroy(triple_index);
 // g_hash_table_destroy(__all_triple_index__);



  ////////// CONSTRUCT KNOWLEDGE BASE //////////



  

/*
  ////////// PERFORM QUERY //////////

  int* variables;
  SddLiteral health_vars = 2, health_vars_count, missing_health_vars;

  // make observations
  delta = sdd_condition(A,delta,m);
  delta = sdd_condition(-C,delta,m);

  // check if observations are normal
  SddNode* gamma;
  gamma = sdd_condition(-faulty1,delta,m);
  gamma = sdd_condition(-faulty2,gamma,m);
  int is_abnormal = gamma == sdd_manager_false(m); // sdd_node_is_false(gamma);
  printf("observations normal?  : %s\n", is_abnormal ? "abnormal":"normal");

  // project onto faults
  SddNode* diagnosis = sdd_exists(B,delta,m);
  // diagnosis no longer depends on variables A,B or C

  // count the number of diagnoses
  SddModelCount count = sdd_model_count(diagnosis,m);
  // adjust for missing faults
  variables = sdd_variables(diagnosis,m);
  health_vars_count = variables[faulty1] + variables[faulty2];
  missing_health_vars = health_vars - health_vars_count;
  count <<= missing_health_vars; // multiply by 2^missing_health_vars
  free(variables);

  // find minimum cardinality diagnoses
  SddNode* min_diagnosis = sdd_minimize_cardinality(diagnosis,m);
  variables = sdd_variables(min_diagnosis,m);
  // adjust for missing faults
  if ( variables[faulty1] == 0 )
    min_diagnosis = sdd_conjoin(min_diagnosis,sdd_manager_literal(-faulty1,m),m);
  if ( variables[faulty2] == 0 )
    min_diagnosis = sdd_conjoin(min_diagnosis,sdd_manager_literal(-faulty2,m),m) ;
  free(variables);



  // count the number of minimum cardinality diagnoses, and minimum cardinality
  SddModelCount min_count = sdd_model_count(min_diagnosis,m);
  SddLiteral min_card =  sdd_minimum_cardinality(min_diagnosis);
  



  printf("sdd model count       : %"PRImcS"\n",count);
  printf("sdd model count (min) : %"PRImcS"\n",min_count);
  printf("sdd cardinality       : %"PRIlitS"\n",min_card);
 */ 
  

  /*
  ////////// SAVE SDDS //////////

  printf("saving sdd and dot ...\n");

  sdd_save("output/circuit-kb.sdd",delta);
  sdd_save("output/diagnosis.sdd",diagnosis);
  sdd_save("output/diagnosis-min.sdd",min_diagnosis);

  sdd_save_as_dot("output/circuit-kb.dot",delta);
  sdd_save_as_dot("output/diagnosis.dot",diagnosis);
  sdd_save_as_dot("output/diagnosis-min.dot",min_diagnosis);

  */
  ////////// CLEAN UP //////////
  //

  //sdd_deref(kg,m);
  //sdd_manager_garbage_collect(m);
  //sdd_manager_free(m);
  //sdd_vtree_free(Vtree);

  //return 0;
//}
