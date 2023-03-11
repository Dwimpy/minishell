CMD1 && CMD2 (execute CMD2 only if CMD1 succeedd)
					&&
				/		\
			CMD1		CMD2

CMD1 || CMD2 (execute CMD2 only if CMD1 fails)
					||
				/		\
			CMD1		CMD2

CMD1 | CMD2 | CMD3

						expression           
					/		|		\		|	
				pipeline    &&		cmd		||
				/					/   \
			  pipe				  echo	 z
			/		\
		  pipe	    cmd
		  /	   \    /  \
		/		\  wc   -l
	  cmd		cmd
	/	\		/  \
   /     \	  cat	empty
echo   hello

cmd | cmd | cmd 
cmd


					expression
				/				\
			pipeline	  |		cmd
			/	|	\	 &&	   /   \
		  cmd1 cmd2 cmd3	 name  args
