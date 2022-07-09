
//Listing 1   A Goertzel implementation


#define PI 3.14

#define SAMPLING_RATE        8000.0        //8kHz
#define TARGET_FREQUENCY        1000.0        //941 Hz
#define N     300        //Block size



int X = 0, Y;
unsigned ADCvalue;
float temp, temp2;
float max = 0;
float max_frq = 0;


double coeff;
double Q1;
double Q2;
double sine;
double cosine;

unsigned char testData[N];

/* Call this routine before every "block" (size=N) of samples. */
void ResetGoertzel(void)
{
  Q2 = 0;
  Q1 = 0;
}

/* Call this once, to precompute the constants. */
void InitGoertzel(void)
{
  int        k;
  double        floatN;
  double        omega;

  floatN = (double)N;
  k = (int)(0.5 + ((floatN * TARGET_FREQUENCY) / SAMPLING_RATE));
  omega = (2.0 * PI * k) / floatN;
  sine =  sin(omega);
  cosine = cos(omega);
  coeff = 2.0 * cosine;

  ResetGoertzel();
}

/* Call this routine for every sample. */
void ProcessSample(unsigned char sample)
{
  double Q0;
  Q0 = coeff * Q1 - Q2 + (double)sample;
  Q2 = Q1;
  Q1 = Q0;
}


/* Basic Goertzel */
/* Call this routine after every block to get the complex result. */
void GetRealImag(double *realPart, double *imagPart)
{
  *realPart = (Q1 - Q2 * cosine);
  *imagPart = (Q2 * sine);
}



/*** End of Goertzel-specific code, the remainder is test code. */

/* Synthesize some test data at a given frequency. */



void sin_wave(double frequency)
{
  int        index;
  double        step;

  step = frequency * ((2.0 * PI) / SAMPLING_RATE);

  /* Generate the test data */
  for (index = 0; index < N; index++)
  {
    testData[index] = (unsigned char)(25.0 * sin(index * step) + 25.0);
    ResetGoertzel();
  }
}
void dumped_sin_wave(double frequency)
{
  int        index;
  double        step;

  step = frequency * ((2.0 * PI) / SAMPLING_RATE);

  /* Generate the test data */
  for (index = 0; index < N; index++)
  {
    testData[index] = (unsigned char)(25.0 * exp(-(double)index * 0.007) * sin(index * step) + 25.0);
    ResetGoertzel();
  }
}
void sin_wave_with_changing_freq(double frequency)
{
  int        index;
  double        step;
  double  frequency1;
  double f_0 = frequency/100.0;

  /* Generate the test data */
  for (index = 0; index < N; index++)
  {
    frequency1 = frequency +  5*sin(PI*index);
    step = frequency1 * ((2.0 * PI) / N);
    testData[index] = (unsigned char)(25.0 * sin(index * step) + 25.0);
    ResetGoertzel();
  }
}


/* Demo */
void GenerateAndTest(double frequency)
{
  int        index;

  double        magnitudeSquared;
  double        magnitude;
  double        real;
  double        imag;
  //sin_wave(frequency);
  //dumped_sin_wave(frequency);
    sin_wave_with_changing_freq(frequency);
  /* Process the samples. */
  for (index = 0; index < N; ++index)
  {
    ProcessSample(testData[index]);
  }

  /* Do the "standard Goertzel" processing. */
  GetRealImag(&real, &imag);

  magnitudeSquared = real * real + imag * imag;
  magnitude = sqrt(magnitudeSquared);

  temp =  magnitude;// / 120.0;
  Serial.println((int)temp);
  if(temp < max)
  {
    max = temp;
    max_frq = frequency;
  }

  ResetGoertzel();
}




void setup() {
  Serial.begin(115200);
}

void loop() {
  double freq;
  InitGoertzel();
  for (freq = TARGET_FREQUENCY - 200; freq <= TARGET_FREQUENCY + 200; freq = freq + 1)
  {
    GenerateAndTest(freq);
  }
  //sin_wave_with_changing_freq(TARGET_FREQUENCY);
  //for(int i = 0; i<300; i++)
   // Serial.println(testData[i]);


  //Serial.println("Max freq at: ");
  //Serial.print(max_frq);
  while (1);
}
