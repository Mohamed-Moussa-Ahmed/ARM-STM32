
u8 CompStr (u8* ptr1 , u8 *ptr2)
{
	u8 iterator =0 ;
	u8 status = 0 ;
	
	while(ptr1[iterator] == ptr2[iterator])
	{
		iterator ++;
		
		if( (ptr1[iterator] == '\0') &&  (ptr2[iterator]= '\0') )
		{
			status = 1;
			break;
		}
		
		else if (((ptr1[iterator] != '\0') &&  (ptr2[iterator]== '\0')) || ((ptr1[iterator] == '\0') &&  (ptr2[iterator]!= '\0')))
		{
			status = 0;
			break;
		}
	}
	
	/*if(ptr1[iterator] != ptr2[iterator])
	status = 0;*/
	
	return status; 
}