#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

// Sta³e dotycz¹ce wymiarów okna i iloœci klatek na sekundê
const int SZEROKOSC = 800;
const int WYSOKOSC = 600;
const int FPS = 60;

// Kolory u¿ywane dla planet
const sf::Color BIALY(255, 255, 255);
const sf::Color ZOLTY(255, 255, 0);
const sf::Color NIEBIESKI(0, 0, 255);
const sf::Color CZERWONY(255, 0, 0);
const sf::Color ZIELONY(0, 255, 0);

// Klasa reprezentuj¹ca planetê
class Planeta : public sf::Drawable, public sf::Transformable {
public:
    // Konstruktor tworz¹cy planetê z okreœlonymi parametrami
    Planeta(const std::string& nazwa, sf::Color kolor, float promien, float odleglosc, float predkosc) :
        m_nazwa(nazwa),
        m_kolor(kolor),
        m_promien(promien),
        m_odleglosc(odleglosc),
        m_predkosc(predkosc),
        m_kat(0)
    {
        // Inicjalizacja w³aœciwoœci okrêgu reprezentuj¹cego planetê
        m_okrag.setRadius(promien);
        m_okrag.setFillColor(kolor);
        m_okrag.setOrigin(promien, promien);

        // Inicjalizacja w³aœciwoœci okrêgu reprezentuj¹cego orbitê planety
        m_orbita.setRadius(odleglosc);
        m_orbita.setFillColor(sf::Color(0, 0, 0, 0));
        m_orbita.setOutlineColor(sf::Color(100, 100, 100));
        m_orbita.setOutlineThickness(1);
        m_orbita.setOrigin(odleglosc, odleglosc);
    }

    // Aktualizacja pozycji planety na podstawie jej prêdkoœci orbitalnej
    void aktualizuj() {
        m_kat += m_predkosc;
        float x = SZEROKOSC / 2 + m_odleglosc * std::cos(3.14 * m_kat / 180);
        float y = WYSOKOSC / 2 + m_odleglosc * std::sin(3.14 * m_kat / 180);
        setPosition(x, y);
    }

private:
    std::string m_nazwa;
    sf::Color m_kolor;
    float m_promien;
    float m_odleglosc;
    float m_predkosc;
    float m_kat;
    sf::CircleShape m_okrag; // Okr¹g reprezentuj¹cy planetê
    sf::CircleShape m_orbita; // Okr¹g reprezentuj¹cy orbitê planety

    // Funkcja rysuj¹ca, u¿ywana do renderowania planety i jej orbity
    virtual void draw(sf::RenderTarget& cel, sf::RenderStates stany) const {
        stany.transform *= getTransform();

        // Rysowanie okrêgu orbity
        cel.draw(m_orbita, stany);

        // Rysowanie okrêgu planety
        cel.draw(m_okrag, stany);

        // Rysowanie nazwy planety jako tekstu
        sf::Text tekst;
        tekst.setString(m_nazwa);
        tekst.setCharacterSize(12);
        tekst.setFillColor(sf::Color::White);
        tekst.setPosition(getPosition().x - m_promien, getPosition().y - m_promien - 15);
        cel.draw(tekst, stany);
    }
};

// G³ówna funkcja, gdzie rozpoczyna siê wykonanie programu
int main() {
    // Utworzenie okna
    sf::RenderWindow okno(sf::VideoMode(SZEROKOSC, WYSOKOSC), "Symulacja Uk³adu S³onecznego");
    okno.setFramerateLimit(FPS);

    // Utworzenie S³oñca na œrodku okna
    sf::CircleShape slonce(25);
    slonce.setFillColor(ZOLTY);
    slonce.setOrigin(25, 25);
    slonce.setPosition(SZEROKOSC / 2, WYSOKOSC / 2);

    // Utworzenie planet na podstawie predefiniowanych danych
    std::vector<Planeta> planety;
    std::vector<std::tuple<std::string, sf::Color, float, float, float>> danePlanet = {
        std::make_tuple("Merkury", NIEBIESKI, 5, 50, 3),
        std::make_tuple("Wenus", BIALY, 8, 80, 2),
        std::make_tuple("Ziemia", ZIELONY, 10, 120, 1.5),
        std::make_tuple("Mars", CZERWONY, 7, 180, 1),
        std::make_tuple("Jowisz", sf::Color::Yellow, 20, 250, 0.5),
    };

    // Inicjalizacja planet na podstawie dostêpnych danych
    for (const auto& data : danePlanet) {
        std::string nazwa;
        sf::Color kolor;
        float promien, odleglosc, predkosc;
        std::tie(nazwa, kolor, promien, odleglosc, predkosc) = data;
        planety.emplace_back(nazwa, kolor, promien, odleglosc, predkosc);
    }

    // G³ówna pêtla gry
    while (okno.isOpen()) {
        // Obs³uga zdarzeñ
        sf::Event zdarzenie;
        while (okno.pollEvent(zdarzenie)) {
            if (zdarzenie.type == sf::Event::Closed) {
                okno.close();
            }
        }

        // Aktualizacja pozycji planet
        for (auto& planeta : planety) {
            planeta.aktualizuj();
        }

        // Wyczyszczenie okna
        okno.clear(sf::Color::Black);

        // Rysowanie S³oñca
        okno.draw(slonce);

        // Rysowanie orbit planet i planet
        for (const auto& planeta : planety) {
            okno.draw(planeta);
        }

        // Wyœwietlenie zawartoœci okna
        okno.display();
    }

    return 0;
}
