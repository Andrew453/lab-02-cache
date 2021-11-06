// Copyright 2021 Andrew Prokushev <senior.prockuschev2017@yandex.ru>

#include <cache.hpp>

/*  Intel Core i5 8265U
 * cache_sizes['1'] = 256 KB
 * cache_sizes['2'] = 1024 KB
 * cache_sizes['3'] = 6144 KB
 * cache_sizes['1']*1/2 = 128 KB < 256 KB < 1024 KB < 6144 KB < 9216 KB =
 * cache_sizes['3']*3/2
 */

CacheDiagnostics::CacheDiagnostics() {
  sizes[0] = 131072 / 4;
  sizes[1] = 262144 / 4;
  sizes[2] = 1048576 / 4;
  sizes[3] = 6291456 / 4;
  sizes[4] = 9437184 / 4;
  statistics[0].bufferSize = sizes[0];
  statistics[1].bufferSize = sizes[1];
  statistics[2].bufferSize = sizes[2];
  statistics[3].bufferSize = sizes[3];
  statistics[4].bufferSize = sizes[4];
}
void CacheDiagnostics::create_array(int num_size) {
  arr = new int[sizes[num_size]];
  srand(time(NULL));
  for (int i = 0; i < sizes[num_size]; i++) {
    arr[i] = rand();
  }
}

void CacheDiagnostics::front_diagnostics(int num_size) {
  //  TODO удалить это поле. сейчас оно чисто для просмотра работоспособности.
  //  last_size = num_size;
  create_array(num_size);
  //  Прогрев
  for (int i = 0; i < sizes[num_size]; i += 16) {
    read_value = arr[i];
  }
  // Чтение + подсчет времени. Время работы  чтения лежит в timer -> после
  // выполнения этой функции надо делать вывод, дабы не потерять данные
  auto start = (double)clock();

  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < sizes[num_size]; j += 16) {
      read_value = arr[j];
    }
  }

  auto end = (double)clock();
  statistics[num_size].duration_front = ((end - start)/CLOCKS_PER_SEC)*1000;
  statistics[num_size].experimentNumber_front = num_size+1;
}

void CacheDiagnostics::reverse_diagnostics(int num_size) {
  create_array(num_size);
  //  Прогрев
  for (int i = 0; i < sizes[num_size]; i += 16) {
    read_value = arr[i];
  }
  // Чтение + подсчет времени. Время работы  чтения лежит в timer -> после
  // выполнения этой функции надо делать вывод, дабы не потерять данные
  auto start = (double)clock();

  for (int i = 0; i < 1000; i++) {
    for (int j = sizes[num_size]; j > 0; j -= 16) {
      read_value = arr[j];
    }
  }
  auto end = (double)clock();
  statistics[num_size].duration_reverse = ((end - start)/CLOCKS_PER_SEC)*1000;
  statistics[num_size].experimentNumber_reverse = num_size+6;
}

void CacheDiagnostics::random_diagnostics(int num_size) {
  create_array(num_size);
  //  Прогрев
  for (int i = 0; i < sizes[num_size]; i += 16) {
    read_value = arr[i];
  }
  std::vector<int> ins;
  for (int i = 0; i < sizes[num_size]; i += 16) {
    ins.push_back(i);
  }
  std::random_shuffle(ins.begin(), ins.end());
  // Чтение + подсчет времени. Время работы  чтения лежит в timer -> после
  // выполнения этой функции надо делать вывод, дабы не потерять данные
 auto start = (double)clock();

  for (int i = 0; i < 1000; i++) {
    for (int j = 0; j < sizes[num_size] / 16; j++) {
      read_value = arr[ins[j]];
    }
  }
 auto end = (double)clock();
  statistics[num_size].duration_random = ((end - start)/CLOCKS_PER_SEC)*1000;
  statistics[num_size].experimentNumber_random = num_size+11;
}
void CacheDiagnostics::formatting_output() {
  std::cout << "  investigaion:\n" <<     "    travel_order: \"front\"\n" <<      "    experiments:\n";
  for (int i=0;i<5;i++) {
    std::cout <<     "    - experiment: " << statistics[i].experimentNumber_front <<    "\n";
    std::cout <<     "        input_data:\n"
                 "          buffer_size: \""<<statistics[i].bufferSize *4/1024<<"Kb\"\n";
    std::cout <<     "        results:\n"
                 "          duration: \""<< statistics[i].duration_front <<"ms\"\n";
  }

  std::cout << "  investigaion:\n" <<     "    travel_order: \"reverse\"\n" <<      "    experiments:\n";
  for (int i=0;i<5;i++) {
    std::cout <<     "    - experiment: " << statistics[i].experimentNumber_reverse <<    "\n";
    std::cout <<     "        input_data:\n"
                 "          buffer_size: \""<<statistics[i].bufferSize *4 /1024<<"Kb\"\n";
    std::cout <<     "        results:\n"
                 "          duration: \""<< statistics[i].duration_reverse <<"ms\"\n";
  }

  std::cout << "  investigaion:\n" <<     "    travel_order: \"random\"\n" <<      "    experiments:\n";
  for (int i=0;i<5;i++) {
    std::cout <<     "    - experiment: " << statistics[i].experimentNumber_random <<    "\n";
    std::cout <<     "        input_data:\n"
                 "          buffer_size: \""<<statistics[i].bufferSize *4/1024<<"Kb\"\n";
    std::cout <<     "        results:\n"
                 "          duration: \""<< statistics[i].duration_random <<"ms\"\n";
  }
}

void CacheDiagnostics::full_diagnostics() {
  for (int i = 0; i < 5; i++) {
    front_diagnostics(i);
  }

  for (int i = 0; i < 5; i++) {
    reverse_diagnostics(i);
  }
  for (int i = 0; i < 5; i++) {
    random_diagnostics(i);
  }
//  formatting_output();
}