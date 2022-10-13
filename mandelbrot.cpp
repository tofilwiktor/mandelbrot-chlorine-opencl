#include "chlorine.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

const int width = 1280;
const int height = 720;

sf::Vector2f complex_to_screen(int width, int height, float re, float im) {
    return sf::Vector2f(((re+2)*(float)width)/3, ((float)height*(im - 1) / -2));
}


const int N3 = (int) pow(256, 3);

sf::Color n_to_rgb(int n, int max_iter) {
	long double n_normal = (long double)n / (long double)max_iter;
        int N = 256;

        n = (int)(n_normal * (long double) N3);

        int g = n / (N*N);

        int n_normal2 = n - g * N * N;

        int r = n_normal2 / N * N;
        int b = n_normal2 - r * N;

        sf::Color col (r, g, b);

        return col;
}


int main()
{
    int iterations = 2000;
    float x_min  = -2;
    float x_max  =  2;
    float y_min  = -1.5f;
    float y_max  =  1.5f;
    float x_step = .002f;
    float y_step = .002f;

    std::vector<float> reals;
    std::vector<float> imags;
    for(float y = y_min; y < y_max; y += y_step)
    for(float x = x_min; x < x_max; x += x_step)
    {
        reals.push_back(x);
        imags.push_back(y);
    }


    std::vector<int> cl_ans(reals.size());
    ch::Worker benoit("mandelbrot.cl");
    benoit.call("solve_mandelbrot", reals, imags, iterations, cl_ans);



    sf::VertexArray va(sf::Points, reals.size());
    sf::RenderWindow window (sf::VideoMode(width, height), "Mandelbrot");

    for (int i = 0; i < reals.size(); i++) {
        va[i].position = complex_to_screen(width, height, reals[i], imags[i]);
        va[i].color = n_to_rgb(cl_ans[i], 100);
    }

    while(window.isOpen()) {
        sf::Event event;

        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();
        window.draw(va);
        window.display();
    }


    return 0;
}
