# Курсовая по ППвИС за третий семестр.

Учитывая то, насколько информативны лекции по МОИС, пример курсача может быть очень полезен
юным разработчикам сверхинтеллектуальных систем.

| № | Путь                                                                            | Шо там лежит                                                                          |
| - | ------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------- |
| 1 | [./kb/longest_path_search/*](./kb/longest_path_search/)                         | SCP программа                                                                         |
| 2 | [./kb/scp_tests/*](./kb/scp_tests/)                                             | Тесты для SCP программы                                                               |
| 3 | [./kb/pseudo-scp_tests/*](./kb/pseudo-scp_tests/)                               | Тесты для псевдо-SCP программы (немного отличаются по формату от SCP тестов)          |
| 4 | [./kb/menu/*](./kb/menu/)                                                       | UI элементы, в том числе добавляющие пункт меню "Запуск SCP программы"                |
| 5 | [./longest_path/\*.*](./longest_path/)                                          | Псевдо-SCP программа                                                                  |
| 6 | [./longest_path/scp_tests_scgs/*](./longest_path/scp_tests_scgs/)               | Тесты для SCP программы в SCg формате, чтобы их можно было посмотреть глязиками в KBE |
| 7 | [./longest_path/pseudo-scp_tests_scgs/*](./longest_path/pseudo-scp_tests_scgs/) | Аналогично предыдущему пункту, но для SCP                                             |

-----------------------------------------------------------------------------------------------------------------

### Пояснения:
* Входная точка SCP программы — [proc_run_all_tests](./kb/longest_path_search/proc_run_all_tests.scs), этот элемент нужно искать в поиске на локалхосте, с него же
проще начинать разбираться в коде, там куча комментариев, должно быть понятно.
* Задание: **поиск максимального пути между двумя узлами ориентированного взвешенного графа.**
* Этот код — переделанный курсач [этого](https://github.com/Yegor-Ikbaev "Ягор") типа,
там тоже можно [посмотреть чё как](https://github.com/Yegor-Ikbaev/scp-chains "тык").
