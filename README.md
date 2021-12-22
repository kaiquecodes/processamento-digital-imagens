# Processamento Digital de Imagens com OpenCV

Esta página tem como objetivo reunir soluções de exercícios propostos na disciplina de Processamento Digital de Imagens do curso de Engenharia da Computação na Universidade Federal do Rio Grande do Norte (UFRN). Em todos os exemplos foi utilizado a linguagem C++ junto com a biblioteca OpenCV.

Acesse: https://kaiquecodes.github.io/processamento-digital-imagens/

Feito por [Kaíquecodes.](https://kaiquecodes.github.io)

# 1.0-Negativo de uma Região

Neste exemplo, temos o programa regions.cpp que recebe do usuário coordenadas cartesianas de dois pontos que devem ser vértices opostos em diagonal. Portanto, com o retângulo criado, será feito a inversão do tom de cinza dos respectivos pixels da região. Para o cálculo da inversão basta implementar o complementar do pixel em questão: C = 255 - P.


<img src="/gitbook/images/ex1-original.png" alt="ex1-original" width = 350px>

###### Figura 1: Foto original RGB.

<img src="/gitbook/images/ex1-resultado.png" alt="ex1-resultado" width = 350px>

###### Figura 2: Foto em GrayScale com o retângulo negativo.

~~~c++
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char** argv){
  Mat image;
  Vec3b val;
  Point p1;
  Point p2;
  String file;
  
  image= imread(argv[1],IMREAD_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu imagem" << std::endl;

  namedWindow("janela", WINDOW_AUTOSIZE);
  
  cout << "Digite as coordenadas de P1: " << endl;

  cin >> p1.x;
  cin >> p1.y; 

  cout << "Digite as coordenadas de P2: " << endl;
 
  cin >> p2.x;
  cin >> p2.y; 

  cout << "Digite o nome do arquivo: " << endl;

  cin >> file;
  
  for(int i=p1.y;i<p2.y;i++){
    for(int j=p1.x;j<p2.x;j++){
      image.at<uchar>(i,j)= 255 - image.at<uchar>(i,j);
    }
  }

  imshow("janela", image);

  imwrite(file + ".png",image);

  waitKey();
  
  return 0;
}

~~~

# 2.0-Troca quadrante.

Aqui está implementado o programa trocaregions.cpp, ele troca os quadrantes em diagonal da imagem de entrada, além disso faz o plot deles e da imagem resultante. Em sua implementação é aproveitado os construtores e métodos disponíveis na biblioteca OpenCV. Em primeiro lugar, é realizado a separação dos quadrantes em quatro imagens,
após isso, é feito a união com o método _copyTo_ já considerando a troca.

![ex2-original](/gitbook/images/ex2-original.png)
###### Figura 3: Foto original RGB.

![ex2-resultado](/gitbook/images/ex2-result.png)
###### Figura 4: Foto em Grayscale com os quadrantes trocados.

~~~c++

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main(int, char** argv){
  Mat image, image1,image2,image3,image4,imageS;

  String file,file1,file2,file3,file4;
  int width=0,height=0;
  
  image= imread(argv[1],IMREAD_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu imagem" << endl;

  namedWindow("janela", WINDOW_AUTOSIZE);

  width = image.cols;
  height = image.rows;

  imageS.create(height,width,CV_8U);
   
 
  image1 = image(Rect(0, 0, width/2, height/2));
  image2 = image(Rect(width/2, 0, width/2, height/2));
  image3 = image(Rect(0, height/2, width/2, height/2));
  image4 = image(Rect(width/2, height/2, width/2, height/2));
 
  cout << "Digite dos nome do arquivo: " << endl;

  cin >> file;
  cin >> file1;
  cin >> file2;
  cin >> file3;
  cin >> file4;
  

  imwrite(file1 + ".png",image1);
  imwrite(file2 + ".png",image2);
  imwrite(file3 + ".png",image3);
  imwrite(file4 + ".png",image4);
 

image4.copyTo(imageS(Rect(0, 0, width/2, height/2))); 

image3.copyTo(imageS(Rect(width/2-1,0, width/2, height/2))); 

image2.copyTo(imageS(Rect(0,height/2-1, width/2, height/2))); 

image1.copyTo(imageS(Rect(width/2-1,height/2-1, width/2, height/2))); 


  imshow("janela", imageS);

  imwrite(file + ".png",imageS);
  
  waitKey();
  
  return 0;
}
~~~

# 3.1: A figura 5 tem mais de 255 bolhas, e agora?

Já que estamos trabalhando em GrayScale, o rótulo poderá ir até 255. Se o número de bolhas for maior que 255, essas não serão contadas. Para resolver esse problema, podemos manipular a imagem em RGB, pois cada canal posseui 255 tons, sendo assim o novo limite de bolhas será 255x255x255.

# 3.2: Preenchendo regiões

Neste exemplo temos a seguinte situação: dada uma imagem conforme a figura 5, realizar a contagem de bolhas que contém buraco e as que não contém buraco. Inicialmente, foi retirada as bolhas que tocam as bordas da imagem visto que não é possível afirmar sobre a presença do buraco nelas. Após isso, foi aplicado o método _floodFill_ para rotular as bolhas com tons de cinzas diferentes ao passo que uma variável auxiliar é incrementada _(nobjects)_. Em seguida, foi utilizado o método _floodFill_ com a semente no ponto (0,0) para o preenchimento do fundo da imagem com a cor 255. Agora, temos o fundo da imagem com a cor branca, as bolhas com tons de cinza diferentes e se há buraco na bolha, podemos garantir que ele tem a cor preta. Por fim, basta realizar uma nova rotulação para contar a quantidade de regiões pretas, para assim determinar o número de bolhas com buracos.

~~~c++
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image,image2,realce;
  int width, height;
  int nobjects;
  
  Point p;
  image = imread(argv[1], IMREAD_GRAYSCALE);
  
  if(!image.data){
    cout << "imagem nao carregou corretamente\n";
    return(-1);
  }

  width=image.cols;
  height=image.rows;
  cout << width << "x" << height << endl;

  p.x=0;
  p.y=0;

//tirando as bolhas que tocam na borda da imagem
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255 && (i == 0 || i == height-1 || j == 0 || j == width-1)){
        p.x=j;
        p.y=i;
        floodFill(image,p,0);
      } 
    }
  }

  image2 = image.clone();

  //busca objetos presentes e rotula
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
        // achou um objeto
        nobjects++;
        p.x=j;
        p.y=i;
  	// preenche o objeto com o contador
	floodFill(image,p,nobjects);
      }
    }
  }

//alterando a cor do fundo
  p.x=0;
  p.y=0;
  floodFill(image,p,255);

//busca objetos presentes e rotula
  int buracos=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0){
        // achou um objeto
        buracos++;
        p.x=j;
        p.y=i;
  		// preenche o objeto com o contador
		  floodFill(image,p,buracos);
      }
    }
  }
  
  cout << "Resultado da Contagem: " << endl;
  cout << "A figura tem " << nobjects - buracos << " bolhas sem buraco" << endl;
  cout << "e " << buracos << " bolhas com buraco." << endl;
  equalizeHist(image, realce);
  imshow("Imagem", image);
  imshow("Imagem sem bolha na borda", image2);
  imwrite("Imagem sem bolha na borda.png",image2);
  imshow("realce", realce);
  imwrite("labeling.png", image);
  waitKey();

  return 0;
} 
~~~

![bolhas](/gitbook/images/bolhas.png)
###### Figura 5: Imagem de entrada.

![bolhas](/gitbook/images/sem-borda.png)
###### Figura 6: Imagem sem as bolhas que tocam a borda.

![imagem-saída](/gitbook/images/labeling.png)
###### Figura 7: Imagem de saída.

![resultado](/gitbook/images/saida.png)
###### Figura 8: Resultado da Contagem.

# 4.1: Equalização de Histograma

Esta é a implementação do programa equalize.cpp, o objetivo dele é a partir de uma imagem de entrada calcular seu histograma, após isso, fazer a técnica de equalização do histograma. Primeiramente, é utilizado a câmera do dispositivo para capturar a imagem, antes de calcular o histograma, é feita a conversão da imagem para tom de cinza. Depois seu histograma é calculado através do método _calcHist_. Por fim, é criado um novo histograma equalizado através do método _equalizeHist_. Ambos os histogramas são plotados na própria janela de exibição da imagem.

~~~c++
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planG; 
  vector<Mat> planE; 
  Mat histR, histE; 
  int nbins = 64; 
  float range[] = {0, 255};
  const float *histrange = { range };
  bool uniform = true; 
  bool acummulate = false;  
  int key;
  Mat gray; 
  Mat equa; 
  
  cap.open(0);

  if(!cap.isOpened()){
    std::cout << "cameras indisponiveis";
    return -1;
  }

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640); 
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2; 
  Mat histImgR(histh, histw, CV_8UC1, Scalar(0,0,0));
  Mat histImgE(histh, histw, CV_8UC1, Scalar(0,0,0));

  
  while(1){
    cap >> image;
    cvtColor(image, gray, COLOR_BGR2GRAY ); //converte grayscale
    split(gray, planG); 
  
    calcHist(&planG[0], 1, 0, Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate); 
                   
    normalize(histR, histR, 0, histImgR.rows, cv::NORM_MINMAX, -1, cv::Mat());
    
    histImgR.setTo(Scalar(0));
    
    for(int i=0; i<nbins; i++){
      line(histImgR,
               Point(i, histh),
               Point(i, histh-cvRound(histR.at<float>(i))),
               Scalar(255), 1, 8, 0);
      
    }
    
    histImgR.copyTo(gray(Rect(0, 0       ,nbins, histh)));
      
    imshow("Grayscale", gray);
     

    equalizeHist(gray, equa ); //equaliza
    split (equa, planE); 
    calcHist(&planE[0], 1, 0, Mat(), histE, 1,
                 &nbins, &histrange,
                 uniform, acummulate); 
                   
    normalize(histE, histE, 0, histImgE.rows, cv::NORM_MINMAX, -1, cv::Mat());
    
    histImgE.setTo(Scalar(0));
    
    for(int i=0; i<nbins; i++){
      line(histImgE,
               Point(i, histh),
               Point(i, histh-cvRound(histE.at<float>(i))),
               Scalar(255), 1, 8, 0);
      
    }
    
    histImgE.copyTo(equa(Rect(0, 0       ,nbins, histh)));
       
    imshow("Equalize",equa);
  
    key = cv::waitKey(30);
    if(key == 27) break;

 }

  return 0;
}

~~~

![ex4-1-original](/gitbook/images/ex4-1-original.png)
###### Figura 9: Imagem em GrayScale juntamente com seu histograma.

![ex4-2-original](/gitbook/images/ex4-2-result.png)
###### Figura 10: Imagem em GrayScale com seu histograma equalizado.

# 4.2: Detecção de Movimentos

Nessa etapa, é feita a detecção de movimentos com o programa motiondetector.cpp utilizando a comparação de dois histogramas calculados em dois instantes diferentes, para isto, é chamado o método _compareHist_ utilizando a comparação por correlação. O programa está calibrado para detectar movimentos quando o retorno do método chamado for menor do que _0.996500_.

~~~c++

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){ 
  Mat image, image2;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes, planes2; 
  Mat histR, histR2;
  int nbins = 64; 
  float range[] = {0, 255}; 
  const float *histrange = { range }; 
  bool uniform = true;  
  bool acummulate = false;  
  int key;

	cap.open(0); 

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  cap.set(CAP_PROP_FRAME_WIDTH, 640); 
  cap.set(CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(CAP_PROP_FRAME_WIDTH);
  height = cap.get(CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2; 
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0)); 
  Mat histImgR2(histh, histw, CV_8UC3, Scalar(0,0,0));
  

  while(1){
    cap >> image;
    split (image, planes2); 
    calcHist(&planes2[0], 1, 0, Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate); 
              

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    

    histImgR.setTo(Scalar(0)); 
       

    for(int i=0; i<nbins; i++){
      line(histImgR,
               Point(i, histh),
               Point(i, histh-cvRound(histR.at<float>(i))),
               Scalar(0, 0, 255), 1, 8, 0);
            
      
    }
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));

    cap >> image2;
    split (image2, planes); 

    calcHist(&planes[0], 1, 0, Mat(), histR2, 1,
                 &nbins, &histrange,
                 uniform, acummulate); 

    normalize(histR2, histR2, 0, histImgR2.rows, NORM_MINMAX, -1, Mat());

    histImgR2.setTo(Scalar(0)); 
    
 for(int i=0; i<nbins; i++){
      
      line(histImgR2,
               Point(i, histh),
               Point(i, histh-cvRound(histR2.at<float>(i))),
               Scalar(0, 0, 255), 1, 8, 0);        
      
    }
   histImgR2.copyTo(image(Rect(0, 2*histh   ,nbins, histh)));


    double comp = compareHist(histR,histR2,0);

    cout << comp << endl;

    if(comp<0.996500){
      putText(image, "Motion Detected", Point(image.cols/2 - 50, 40), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0, 0, 255), 2);
    }

    imshow("image", image);
    key = waitKey(30);
    if(key == 27) break;
  }
  return 0;
}
~~~

![motion](/gitbook/images/Motion.gif)
###### Figura 11: Resultado do Detector de Movimentos.

# 5: Filtro Laplaciano do Gaussiano
O objetivo desse exemplo é demostrar o filtro espacial laplaciano do gaussiano. No programa laplgauss.cpp, há um menu de filtros que podem ser aplicados via escolha do teclado, caso o usuário digitar a tecla k, a máscara setada será a do filtro laplaciano do gaussiano. Comparando as figuras 9 e 10, vemos que o filtro laplaciano do gaussiano acentua ainda mais os contornos das imagens.  

~~~c++
#include <iostream>
#include <opencv2/opencv.hpp>

void printmask(cv::Mat &m) {
  for (int i = 0; i < m.size().height; i++) {
    for (int j = 0; j < m.size().width; j++) {
      std::cout << m.at<float>(i, j) << ",";
    }
    std::cout << "\n";
  }
}

int main(int, char **) {
  cv::VideoCapture cap;  // open the default camera
  float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                   0.1111, 0.1111, 0.1111, 0.1111};
  float gauss[] = {0.0625, 0.125,  0.0625, 0.125, 0.25,
                   0.125,  0.0625, 0.125,  0.0625};
  float horizontal[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  float vertical[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
  float laplacian[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
  float boost[] = {0, -1, 0, -1, 5.2, -1, 0, -1, 0};
  float laplgauss [] ={0,0,-1,0,0,0,-1,-2,-1,0,-1,-2,16,-2,-1,
                      0,-1,-2,-1,0,0,0,-1,0,0};

  cv::Mat frame, framegray, frame32f, frameFiltered;
  cv::Mat mask(3, 3, CV_32F);
  cv::Mat result;
  double width, height;
  int absolut;
  char key;

  cap.open(0);

  if (!cap.isOpened())  // check if we succeeded
    return -1;

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  std::cout << "largura=" << width << "\n";
  ;
  std::cout << "altura =" << height << "\n";
  ;
  std::cout << "fps    =" << cap.get(cv::CAP_PROP_FPS) << "\n";
  std::cout << "format =" << cap.get(cv::CAP_PROP_FORMAT) << "\n";

  cv::namedWindow("filtroespacial", cv::WINDOW_NORMAL);
  cv::namedWindow("original", cv::WINDOW_NORMAL);

  mask = cv::Mat(3, 3, CV_32F, media);

  absolut = 1;  // calcs abs of the image

  for (;;) {
    cap >> frame;  // get a new frame from camera
    cv::cvtColor(frame, framegray, cv::COLOR_BGR2GRAY);
    cv::flip(framegray, framegray, 1);
    cv::imshow("original", framegray);
    framegray.convertTo(frame32f, CV_32F);
    cv::filter2D(frame32f, frameFiltered, frame32f.depth(), mask,
                 cv::Point(1, 1), 0);
    if (absolut) {
      frameFiltered = cv::abs(frameFiltered);
    }

    frameFiltered.convertTo(result, CV_8U);

    cv::imshow("filtroespacial", result);

    key = (char)cv::waitKey(10);
    if (key == 27) break;  // esc pressed!
    switch (key) {
      case 'a':
        absolut = !absolut;
        break;
      case 'm':
        mask = cv::Mat(3, 3, CV_32F, media);
        printmask(mask);
        break;
      case 'g':
        mask = cv::Mat(3, 3, CV_32F, gauss);
        printmask(mask);
        break;
      case 'h':
        mask = cv::Mat(3, 3, CV_32F, horizontal);
        printmask(mask);
        break;
      case 'v':
        mask = cv::Mat(3, 3, CV_32F, vertical);
        printmask(mask);
        break;
      case 'l':
        mask = cv::Mat(3, 3, CV_32F, laplacian);
        printmask(mask);
        break;
      case 'b':
        mask = cv::Mat(3, 3, CV_32F, boost);
        break;
      case 'k':
        mask = cv::Mat(5, 5, CV_32F, laplgauss);
        break;
      default:
        break;
    }
  }
  return 0;
}
~~~

![original](/gitbook/images/original.png)
###### Figura 12: Imagem de Entrada do Algoritmo.

![llaplaciano](/gitbook/images/laplaciano.png)
###### Figura 13: Resultado do Filtro Laplaciano.

![lapgauss](/gitbook/images/lapgauss.png)
###### Figura 14: Resultado do Filtro Laplaciano do Gaussiano.


# 6: Quantização vetorial com k-means

Foi solicitado a implementação do Algoritmo do k-means com o parâmetro NRodadas=1, com inicialização dos centros com KMEANS_RANDOM_CENTERS ao invés de KMEANS_PP_CENTERS e a realização de 10 rodadas. Como resultado coletamos 10 imagens, cada uma correnspondendo a uma rodada do algoritmo e unimos em um arquivo GIF. Como podemos ver, a diferença das imagens é bem acentuada isso acontece pois os centros dos agrupamentos foram escolhidos de forma aleatória em cada rodada com a escolha do KMEANS_RANDOM_CENTERS.

![original](/gitbook/images/Frutas.gif)
###### Figura 15: Gif com o resultado das 10 rodadas.

~~~c++
#include <opencv2/opencv.hpp>
#include <cstdlib>

using namespace cv;

int main( int argc, char** argv ){
  int nClusters = 8;
  Mat rotulos;
  int nRodadas = 1;
  Mat centros;

  if(argc!=3){
	exit(0);
  }

  Mat img = imread( argv[1], IMREAD_COLOR );
  Mat samples(img.rows * img.cols, 3, CV_32F);

  for (int i=0; i<10; i++){

  for( int y = 0; y < img.rows; y++ ){
    for( int x = 0; x < img.cols; x++ ){
      for( int z = 0; z < 3; z++){
        samples.at<float>(y + x*img.rows, z) = img.at<Vec3b>(y,x)[z];
	  }
	}
  }

  kmeans(samples,
		 nClusters,
		 rotulos,
		 TermCriteria(TermCriteria::MAX_ITER|TermCriteria::EPS, 10000, 0.0001),
		 nRodadas,
		 KMEANS_RANDOM_CENTERS,
		 centros );


  Mat rotulada( img.size(), img.type() );
  for( int y = 0; y < img.rows; y++ ){
    for( int x = 0; x < img.cols; x++ ){
	  int indice = rotulos.at<int>(y + x*img.rows,0);
	  rotulada.at<Vec3b>(y,x)[0] = (uchar) centros.at<float>(indice, 0);
	  rotulada.at<Vec3b>(y,x)[1] = (uchar) centros.at<float>(indice, 1);
	  rotulada.at<Vec3b>(y,x)[2] = (uchar) centros.at<float>(indice, 2);
	}
  }
  imshow( "clustered image", rotulada );
  sprintf(argv[2],  "kmeans%d.jpg", i);
  imwrite(argv[2], rotulada);

}  
  waitKey( 0 );
}
~~~

# Exemplo 7: Detecção de bordas com o Algoritmo de Canny.

No exemplo proposto, foi implementado um programa que pega a saída do Algortimo de Canny como entrada para a técnica do pontilhismo. Na imagem resultado temos um STEP=5, JITTER=3 e RAIO=1. 

<img src="/gitbook/images/dindin.png" alt="ex1-original" width = 450px>
###### Figura 16: Aladdin, imagem resultado dos Algoritmos de Canny e Pontilhismo.

~~~c++
#include <numeric>
#include <ctime>
#include <cstdlib>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 1

int top_slider = 1;
int slider_max = 100;

char TrackbarName[50];

Mat image, border,cannypot;
  int width, height;
  Vec3b color;
  int x, y;
  vector<int> yrange;
  vector<int> xrange;

void canny(int, void*){

  Canny(image, border, top_slider, 3*top_slider);
   
  cannypot = Mat(height, width, CV_8UC3, Scalar(255,255,255));
  for(int i =0; i<width; i++){
    for(int j =0; j<height; j++){
      if(border.at<uchar>(j,i) == 255)
      {
          x = i+rand()%(2*JITTER)-JITTER+1;
          y = j+rand()%(2*JITTER)-JITTER+1;
          color = image.at<Vec3b>(y,x);
          circle(cannypot,
                 Point(x,y),
                 RAIO,
                 CV_RGB(color[2],color[1],color[0]),
                 -1,
                 LINE_AA);
      }
    }
  }
  imshow("canny",cannypot);

}

int main(int argc, char**argv){

  image= imread(argv[1],IMREAD_COLOR);
  
  srand(time(0));

  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  
  sprintf(TrackbarName,"Threshold_inferior-%d",slider_max);
  namedWindow("canny",1);
  createTrackbar( TrackbarName, "canny",
                &top_slider,
                slider_max,
                canny );
           
  canny(top_slider, 0 );

  waitKey();
  imwrite("cannypot.png",cannypot);
  return 0;
}

~~~




