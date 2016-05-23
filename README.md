# RayTracing
Simple Ray Tracing

Содержание проекта:

	   1. RayTracing - основная библиотека
	   2. SyncContainer, ThreadPool - Потокобезопасный контейнер и трэд пулл соответственно. Зависимости - boost unit test framework 
	   3. GL_View - Отрисовка в реальном времени. Зависимости - glut.
	      Использование ./GL_View WIDTH HEIGHT CONFIG_FILE SCENE_FILE, где
	      		    WIDTH HEIGHT - размеры изображения
			        CONFIG_FILE - текстовый файл со следующего вида:
    CustomSerializer #Формат сцены CustomSerializer - реализация формата rt; STLBinarySerializer - бинарный STL
    #disableLight # отключить освещение
    #disableReflection # отключить отражение
    #disableRefraction # отключить преломление
    reflectionDepth 20 # Глубина отражения
    refractionDepth 20 # Глубина преломления
    		       	 SCENE_FILE - файл сцены. Дополнительно поддерживается:
    stlmodel 		        #Модель в формате STL бинарном
	    filename /Absolute/path/to/model.stl
	    move 0 30 5	        #Позиция на сцене
        material material_id    #Материал
    endstlmodel
       4. TGAView - Рендеринг в картинку формата tga. Использование аналогично Gl_View с доп. параметром - путём для картинки. По дефолту rendered.tga
	   5. Export - Экспорт из дополненного формата в чистый rt.
Примеры конфигурационного файла в корне проекта.

Необходимые пакеты в Ubunutu:
	
	- libboost-all-dev
	- freeglut3-dev
	- libglew-dev
	- libxmu-dev
	- libxi-dev
	
