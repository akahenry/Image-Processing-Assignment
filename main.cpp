/// Operations ToolKit: JPG/PNG/JPEG/BMP -> void/JPG
//  Created by Henry Bernardo Kochenborger de Avila.
#include <string>
#include <iostream>
#include <TGUI/TGUI.hpp>

#define SCALESNUMBER 255
#define NBUTTONS 6
#define SIZEBUTTONX 300

class Image
{
    sf::Image img;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    /// loadFromFile: std::string -> void
    //  Objective: receives a string which is the filename and load it
    //      to img, texture and sprite.
    void loadFromFile(std::string nof)
    {
        img.loadFromFile(nof);
        texture.loadFromImage(img);
        sprite.setTexture(texture);
    }

    /// getSprite: void -> sf::Sprite
    //  Objective: returns sprite (sf::Sprite) attribute.
    sf::Sprite getSprite()
    {
        return sprite;
    }

    /// getTexture: void -> sf::Texture
    //  Objective: returns texture (sf::Texture) attribute.
    sf::Texture getTexture()
    {
        return texture;
    }

    /// getImage: void -> sf::Image
    //  Objective: returns image (sf::Image) attribute.
    sf::Image getImage()
    {
        return img;
    }

    /// updateImage: sf::Image -> void
    //  Objective: given an sf::Image object, this function updates the referenced instance
    //      with this image.
    void updateImage(sf::Image image)
    {
        img = image;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
    }

    /// copy: Image -> void
    //  Objective: given an Image object, this function attributes to the referenced instance.
    void copy(Image src)
    {
        Image::updateImage(src.getImage());
    }

    /// saveToFile: std::string -> void
    //  Objective: given a string, this function saves this instance in the same directory
    //      and the given string.
    void saveToFile(std::string filename)
    {
        img.saveToFile(filename);
    }

    /// getSize: void -> sf::Vector2u
    //  Objective: this function returns the image's size.
    sf::Vector2u getSize()
    {
        return img.getSize();
    }

    /// setPosition: float float -> void
    //  Objective: this function set the sprite's position (x,y) to the given parameters.
    void setPosition(float x, float y)
    {
        sprite.setPosition(x,y);
    }

    /// flipVertical: void -> void
    //  Objective: this function rotates the image in 180º.
    void flipVertical()
    {
        sf::Color pixel;
        sf::Vector2u sizeImg = img.getSize();

        for(unsigned int x=0; x < sizeImg.x; x++)
        {
            for(unsigned int y=0; y < sizeImg.y/2; y++)
            {
                pixel = img.getPixel(x, y);
                img.setPixel(x, y, img.getPixel(x, sizeImg.y-y-1));
                img.setPixel(x, sizeImg.y-y-1, pixel);
            }
        }
        updateImage(img);
    }

    /// flipHorizontal: void -> void
    //  Objective: this function does a horizontal flip on the image.
    //  Note: It works like a mirror.
    void flipHorizontal()
    {
        sf::Color pixel;
        sf::Vector2u sizeImg = img.getSize();

        for(unsigned int x=0; x < sizeImg.x/2; x++)
        {
            for(unsigned int y=0; y < sizeImg.y; y++)
            {
                pixel = img.getPixel(x, y);
                img.setPixel(x, y, img.getPixel(sizeImg.x-x-1, y));
                img.setPixel(sizeImg.x-x-1, y, pixel);
            }
        }
        updateImage(img);
    }

    /// toGreyScale: void -> void
    //  Objective: this function changes the image to gray scale using for all the three channels:
    //      Pixel = 0.299*RED + GREEN*0.587 + BLUE*0.114
    void toGreyScale()
    {
        sf::Color currentPixel;
        unsigned int color;
        sf::Vector2u sizeImg = img.getSize();

        for(unsigned int x=0; x < sizeImg.x; x++)
        {
            for(unsigned int y=0; y < sizeImg.y; y++)
            {
                currentPixel = img.getPixel(x,y);
                color = round(float(currentPixel.r)*0.299 + float(currentPixel.g)*0.587 + float(currentPixel.b)*0.114);
                img.setPixel(x,y, sf::Color(color, color, color));
            }
        }
        updateImage(img);
    }

    /// quantize: unint -> void
    //  Objective: this function changes the image according the parameter. It defines how many
    //      shades of grey will be used. This is calculated based on rounding for floats to
    //      integers.
    void quantize(unsigned int scales)
    {
        sf::Color currentPixel;
        int color;
        sf::Vector2u sizeImg = img.getSize();

        for(unsigned int x=0; x < sizeImg.x; x++)
        {
            for(unsigned int y=0; y < sizeImg.y; y++)
            {
                currentPixel = img.getPixel(x,y);
                if(scales == 1)
                    color = 0;
                else
                    color = round(round(float(currentPixel.r)/255*float(scales-1))*255/float(scales-1));
                img.setPixel(x,y, sf::Color(color, color, color));
            }
        }
        updateImage(img);
    }
};

int main(int argc, char *argv[])
{
    unsigned int scales = 0;
    Image original, changed;
    original.loadFromFile(argv[1]);
    changed.loadFromFile(argv[1]);
    changed.setPosition(original.getSize().x, 0);
    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(original.getSize().x*2 + SIZEBUTTONX, original.getSize().y), "OP Toolkit");

    /// GUI's stuff
    tgui::Gui gui{window};

    tgui::Button::Ptr flipVertical = tgui::Button::create("Flip Vertical");
    flipVertical->setSize(SIZEBUTTONX, original.getSize().y/NBUTTONS);
    flipVertical->setPosition(original.getSize().x*2, 0);
    flipVertical->connect("pressed", &Image::flipVertical, &changed);

    tgui::Button::Ptr flipHorizontal = tgui::Button::create("Flip Horizontal");
    flipHorizontal->setSize(SIZEBUTTONX, original.getSize().y/NBUTTONS);
    flipHorizontal->setPosition(original.getSize().x*2, original.getSize().y/NBUTTONS);
    flipHorizontal->connect("pressed", &Image::flipHorizontal, &changed);

    tgui::Button::Ptr greyScale = tgui::Button::create("Grey Scale");
    greyScale->setSize(SIZEBUTTONX, original.getSize().y/NBUTTONS);
    greyScale->setPosition(original.getSize().x*2, 2*original.getSize().y/NBUTTONS);
    greyScale->connect("pressed", &Image::toGreyScale, &changed);

    tgui::Button::Ptr copyButton = tgui::Button::create("Copy");
    copyButton->setSize(SIZEBUTTONX, original.getSize().y/NBUTTONS);
    copyButton->setPosition(original.getSize().x*2, 3*original.getSize().y/NBUTTONS);
    copyButton->connect("pressed", &Image::copy, &changed, original);

    tgui::Slider::Ptr quantizeSlider = tgui::Slider::create(0, SCALESNUMBER);
    quantizeSlider->setSize(SIZEBUTTONX, (original.getSize().y/NBUTTONS)/2);
    quantizeSlider->setPosition(original.getSize().x*2, 4*original.getSize().y/NBUTTONS);
    quantizeSlider->setMaximum(256);
    quantizeSlider->setMinimum(1);
    quantizeSlider->setStep(1);

    tgui::Button::Ptr quantizeButton = tgui::Button::create("Quantize");
    quantizeButton->setSize(SIZEBUTTONX*3/4, (original.getSize().y/NBUTTONS)/2);
    quantizeButton->setPosition(original.getSize().x*2, original.getSize().y/NBUTTONS*9/2);
    quantizeButton->connect("pressed", &Image::quantize, &changed, std::ref(scales));

    tgui::TextBox::Ptr quantizeNumber = tgui::TextBox::create();
    quantizeNumber->setSize(SIZEBUTTONX*1/4, (original.getSize().y/NBUTTONS)/2);
    quantizeNumber->setPosition(original.getSize().x*2+SIZEBUTTONX*3/4, original.getSize().y/NBUTTONS*9/2);
    quantizeNumber->setReadOnly(true);

    tgui::Button::Ptr saveButton = tgui::Button::create("Save");
    saveButton->setSize(SIZEBUTTONX, original.getSize().y/NBUTTONS);
    saveButton->setPosition(original.getSize().x*2, 5*original.getSize().y/NBUTTONS);
    saveButton->connect("pressed", &Image::saveToFile, &changed, "output.jpg");

    gui.add(flipHorizontal);
    gui.add(flipVertical);
    gui.add(greyScale);
    gui.add(copyButton);
    gui.add(quantizeNumber);
    gui.add(quantizeButton);
    gui.add(quantizeSlider);
    gui.add(saveButton);

    /// Window's loop
    while (window.isOpen())
    {
        scales = (unsigned int)(quantizeSlider->getValue());
        quantizeNumber->setText(std::to_string(scales));

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            gui.handleEvent(event);
        }

        window.clear(sf::Color(255,255,255));
        window.draw(original.getSprite());
        window.draw(changed.getSprite());
        gui.draw();
        window.display();
    }

    return 0;
}
