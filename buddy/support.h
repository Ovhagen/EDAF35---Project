
short calc_minimum_power(int memory_size){
  int power = 0;
  memory_size = memory_size/2;
  while(memory_size > 16){
    power++;;
    memory_size = memory_size/2;
  }

  short MIN_DATA_SIZE = 1;
  for(int i = 0; i < power; i++)
    MIN_DATA_SIZE *= 2;
  return MIN_DATA_SIZE;
}
