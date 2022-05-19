# Goertzel-Algorithm
TL:DR - An efficient way to implement FFT using Arduino.

May 31 2021, Roy Ben Avraham;Mk;Ge

First some theory about Goertzel Algorithm:
(For the project itself, scroll down)

The Goertzel algorithm can perform tone detection using much less CPU horsepower than the Fast Fourier Transform, but many engineers have never heard of it. This article attempts to change that.

Most engineers are familiar with the Fast Fourier Transform (FFT) and would have little trouble using a “canned” FFT routine to detect one or more tones in an audio signal. What many don't know, however, is that if you only need to detect a few frequencies, a much faster method is available. It's called the Goertzel algorithm.

Tone detection
Many applications require tone detection, such as:

DTMF (touch tone) decoding
Call progress (dial tone, busy, and so on) decoding
Frequency response measurements (sending a tone while simultaneously reading back the result)-if you do this for a range of frequencies, the resulting frequency response curve can be informative. For example, the frequency response curve of a telephone line tells you if any load coils (inductors) are present on that line.
Although dedicated ICs exist for the applications above, implementing these functions in software costs less. Unfortunately, many embedded systems don't have the horsepower to perform continuous real-time FFTs. That's where the Goertzel algorithm comes in.

In this article, I describe what I call a basic Goertzel and an optimized Goertzel.

The basic Goertzel gives you real and imaginary frequency components as a regular Discrete Fourier Transform (DFT) or FFT would. If you need them, magnitude and phase can then be computed from the real/imaginary pair.

The optimized Goertzel is even faster (and simpler) than the basic Goertzel, but doesn't give you the real and imaginary frequency components. Instead, it gives you the relative magnitude squared. You can take the square root of this result to get the relative magnitude (if needed), but there's no way to obtain the phase.

In this short article, I won't try to explain the theoretical background of the algorithm. I do give some links at the end where you can find more detailed explanations. I can tell you that the algorithm works well, having used it in all of the tone detection applications previously listed (and others).

A basic Goertzel
First a quick overview of the algorithm: some intermediate processing is done in every sample. The actual tone detection occurs every Nth sample. (I'll talk more about N in a minute.)

As with the FFT, you work with blocks of samples. However, that doesn't mean you have to process the data in blocks. The numerical processing is short enough to be done in the very interrupt service routine (ISR) that is gathering the samples (if you're getting an interrupt per sample). Or, if you're getting buffers of samples, you can go ahead and process them a batch at a time.

Before you can do the actual Goertzel, you must do some preliminary calculations:

Decide on the sampling rate.
Choose the block size, N.
Precompute one cosine and one sine term.
Precompute one coefficient.
These can all be precomputed once and then hardcoded in your program, saving RAM and ROM space; or you can compute them on-the-fly.

Sampling rate
Your sampling rate may already be determined by the application. For example, in telecom applications, it's common to use a sampling rate of 8kHz (8, 000 samples per second). Alternatively, your analog-to-digital converter (or CODEC) may be running from an external clock or crystal over which you have no control.

If you can choose the sampling rate, the usual Nyquist rules apply: the sampling rate will have to be at least twice your highest frequency of interest. I say “at least” because if you are detecting multiple frequencies, it's possible that an even higher sampling frequency will give better results. What you really want is for every frequency of interest to be an integer factor of the sampling rate.

Block size
Goertzel block size N is like the number of points in an equivalent FFT. It controls the frequency resolution (also called bin width). For example, if your sampling rate is 8kHz and N is 100 samples, then your bin width is 80Hz.

This would steer you towards making N as high as possible, to get the highest frequency resolution. The catch is that the higher N gets, the longer it takes to detect each tone, simply because you have to wait longer for all the samples to come in. For example, at 8kHz sampling, it will take 100ms for 800 samples to be accumulated. If you're trying to detect tones of short duration, you will have to use compatible values of N.

The third factor influencing your choice of N is the relationship between the sampling rate and the target frequencies. Ideally you want the frequencies to be centered in their respective bins. In other words, you want the target frequencies to be integer multiples of sample_rate/N.

The good news is that, unlike the FFT, N doesn't have to be a power of two.

Precomputed constants
Once you've selected your sampling rate and block size, it's a simple five-step process to compute the constants you'll need during processing:

![image](https://user-images.githubusercontent.com/105777016/169200966-ec8cd242-e601-4073-a7c4-e04e335d91a8.png)

w= (2*π/N)*kcosine= coswsine= sinwcoeff= 2 *cosine

For the per-sample processing you're going to need three variables. Let's call them Q0,Q1, and Q2.

Q1 is just the value of Q0last time.Q2 is just the value of Q0two times ago (or Q1 last time).

Q1 and Q2 must be initialized to zero at the beginning of each block of samples. For every sample, you need to run the following three equations:

Q0= coeff * Q1– Q2+ sampleQ2= Q1Q1= Q0

After running the per-sample equations N times, it's time to see if the tone is present or not.

real = (Q1– Q2* cosine)imag = (Q2* sine)magnitude2= real2+ imag2

A simple threshold test of the magnitude will tell you if the tone was present or not. Reset Q2 and Q1 to zero and start the next block.

An optimized Goertzel
The optimized Goertzel requires less computation than the basic one, at the expense of phase information.

The per-sample processing is the same, but the end of block processing is different. Instead of computing real and imaginary components, and then converting those into relative magnitude squared, you directly compute the following:

magnitude2= Q12+ Q22-Q1*Q2*coeff

This is the form of Goertzel I've used most often, and it was the first one I learned about.

Source: https://www.embedded.com/the-goertzel-algorithm/

What will we do:
Goals:

1. Prepare TinkerCad

Arduino Simulation that implements Goertzel Algorithm

2. Prepare at least 3 examples demonstrating usefulness of the Goertzel Algorithm.

(use at least plain sin wave, dumped sin wave and dumped sin wave with changing frequency)

## A little bit results:
Sin wave:

![image](https://user-images.githubusercontent.com/105777016/169201002-692a64f2-0205-4a0a-ae01-a97a41525773.png)

Harmony Amplitude in f=1000[Hz] is according to: (amplitude*N)/2 = (25*300)/2=3750

The results We got were:

![image](https://user-images.githubusercontent.com/105777016/169201033-9ca6e906-2091-4054-9265-20a174ec7e3b.png)

Which is pretty much as in theory (really small deviations).

Dump Sin Wave:

From MATLAB:

![image](https://user-images.githubusercontent.com/105777016/169201064-60ca719a-de04-44f6-a478-4e4876d56e5b.png)
(Dumped sine wave from MATLAB)

Result of FFT for dumpsin wave:

![image](https://user-images.githubusercontent.com/105777016/169201118-8b4782a2-ee0f-4484-b9bc-fb699a9357ec.png)

And using Goertzel Algorithm we get:

![image](https://user-images.githubusercontent.com/105777016/169201142-b1fdd697-e7cb-4676-a554-a56d6b31617e.png)
(Result of calculating frequencies spectrum)

Sin wave with changing frequency:

from MATLAB:

![image](https://user-images.githubusercontent.com/105777016/169201196-d2f8e4ab-88df-4b1e-aa76-97faa4282b1e.png)
(Sine wave with changing frequency)

and using Goertzel Algorithm:

![image](https://user-images.githubusercontent.com/105777016/169201240-07d09577-a5df-4551-8668-3c4349e8312d.png)
(Frequencies spectrum)




