# При добавлении новой библиотеки впишите её название здесь.
# Название библиотеки должно совпадать с названием папки, в которой она лежит.
set(INTERNAL_LIBS
	ExampleLibrary2
	ExampleLibrary
	net
)

foreach(dir ${INTERNAL_LIBS})
	add_subdirectory(${dir})
endforeach()

# Robot - особая библиотека, потому что к ней подключаются все остальные.
# Вроде бы циклические зависимости разрешаются без проблем, так что в зависимости остальным библиотекам
# можно спокойно писать любые другие.
add_subdirectory("Robot")
list(APPEND INTERNAL_LIBS Robot)
set(INTERNAL_LIBS ${INTERNAL_LIBS} PARENT_SCOPE)
