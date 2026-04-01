#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace sf;
using namespace std;

ConvexShape HCN_bo_goc(Vector2f size, float radius, Color color, Vector2f position)
{
    const int Diem_ve = 20; // Điểm mỗi góc
    ConvexShape shape;
    shape.setPointCount(Diem_ve * 4); // Tổng các điểm vẽ

    float pi = 3.141592654;
    float goc_moi_cung = 90.f / (Diem_ve - 1); // chia 90 độ thành các cung vs góc nhỏ hơn
    for (int i = 0; i < Diem_ve * 4; i++)
    {
        int Tam_goc = i / Diem_ve;      // tâm góc thứ mấy
        int Diem_thu_may = i % Diem_ve; // Điểm thứ mấy trong góc đó

        // Tinh góc quay từ điểm bắt đầu
        float goc = Tam_goc * 90.f + Diem_thu_may * goc_moi_cung;

        Vector2f Tam; // vector lưu vị trí tâm cung tròn

        // Tính tâm;
        if (Tam_goc == 0)
        {
            Tam = {size.x - radius, size.y - radius};
        }
        else if (Tam_goc == 1)
        {
            Tam = {radius, size.y - radius};
        }
        else if (Tam_goc == 2)
        {
            Tam = {radius, radius};
        }
        else
        {
            Tam = {size.x - radius, radius};
        }

        // Toạ độ điểm vẽ

        float x = Tam.x + radius * cos(goc * pi / 180.f);
        float y = Tam.y + radius * sin(goc * pi / 180.f);

        shape.setPoint(i, {x, y});
    }
    shape.setFillColor(color);
    shape.setPosition(position);
    return shape;
}

struct Colordata
{
    string name;
    Color color;
};

struct Bi
{
    CircleShape bi;
    int cot;
    Vector2f dich;
    int status = 0; // 0.đứng yên,1.Bay lên/Bay xuống(Y),2.Bay ngang(X),3.Về vị trí(Y)
    float h;
};

int main()
{
    RenderWindow Window(VideoMode({1200, 850}), "PBL");
    Window.setFramerateLimit(60);

    vector<Colordata> Mau = {{"Orange", Color(255, 165, 0)}, {"Yellow", Color::Yellow}, {"Green", Color::Green}, {"Blue", Color::Blue}, {"Indigo", Color(75, 0, 130)}, {"Violet", Color(238, 130, 238)}};

    ConvexShape nen_chinh = HCN_bo_goc({1100, 750}, 30, Color(17, 29, 59), {50, 50});

    Font font("ARIAL.TTF");
    Text title1(font, "BAM DE DOI MAU", 25);
    title1.setPosition({130, 110});
    title1.setStyle(Text::Bold);
    Text title2(font, "BAM DE TAO BI", 25);
    title2.setPosition({130, 360});
    title2.setStyle(Text::Bold);

    ConvexShape nut = HCN_bo_goc({200, 60}, 15, Color(46, 204, 113), {850, 150});
    Text start(font, "BAT DAU SORT", 25);
    start.setPosition({854, 162});
    start.setStyle(Text::Bold);

    int Mau_hien_tai[3] = {0, 3, 4};
    vector<ConvexShape> o_chon_mau, o_them_bi;
    vector<Bi> ds_bi;
    bool click = false;

    vector<pair<int, int>> fswap;

    for (int i = 0; i < 3; i++)
    {
        o_chon_mau.push_back(HCN_bo_goc({150, 90}, 20, Mau[Mau_hien_tai[i]].color, {130.f + i * 180.f, 150.f}));
        o_them_bi.push_back(HCN_bo_goc({150, 90}, 20, Mau[Mau_hien_tai[i]].color, {130.f + i * 180.f, 400.f}));
    }

    Clock clock;

    while (Window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        while (auto event = Window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                Window.close();
            }

            if (auto mouse = event->getIf<Event::MouseButtonPressed>())
            {
                if (mouse->button == Mouse::Button::Left)
                {
                    Vector2f pos = Window.mapPixelToCoords(Mouse::getPosition(Window));

                    for (int i = 0; i < 3; i++)
                    {
                        if (o_chon_mau[i].getGlobalBounds().contains(pos))
                        {
                            int check = Mau_hien_tai[i];
                            bool oke;
                            do
                            {
                                oke = false;
                                check = (check + 1) % Mau.size();
                                for (int j = 0; j < 3; j++)
                                {
                                    if (i != j && check == Mau_hien_tai[j])
                                    {
                                        oke = true;
                                    }
                                }

                            } while (oke);
                            Mau_hien_tai[i] = check;
                            Color new_color = Mau[check].color;
                            o_chon_mau[i].setFillColor(new_color);
                            o_them_bi[i].setFillColor(new_color);
                            for (auto &x : ds_bi)
                            {
                                if (x.cot == i)
                                {
                                    x.bi.setFillColor(new_color);
                                }
                            }
                        }
                        if (o_them_bi[i].getGlobalBounds().contains(pos))
                        {
                            Bi tmp;
                            tmp.bi.setRadius(25);
                            tmp.bi.setFillColor(o_them_bi[i].getFillColor());
                            tmp.bi.setOrigin({25, 25});
                            tmp.bi.setPosition({150.f + ds_bi.size() * 60.f, 680.f});
                            tmp.cot = i;
                            ds_bi.push_back(tmp);
                        }
                    }
                    if (nut.getGlobalBounds().contains(pos))
                    {
                        fswap.clear();
                        int n = ds_bi.size();
                        vector<int> vi_tri(n);
                        for (int i = 0; i < n; i++)
                        {
                            vi_tri[i] = i;
                        }

                        int i = 0, j = 0, k = n - 1;
                        while (j <= k)
                        {
                            if (ds_bi[vi_tri[j]].cot == 0)
                            {
                                if (i != j)
                                {
                                    fswap.push_back({vi_tri[i], vi_tri[j]});
                                    swap(vi_tri[i], vi_tri[j]);
                                }
                                ++i, ++j;
                            }
                            else if (ds_bi[vi_tri[j]].cot == 1)
                            {
                                ++j;
                            }
                            else
                            {
                                if (j != k)
                                {
                                    fswap.push_back({vi_tri[j], vi_tri[k]});
                                    swap(vi_tri[j], vi_tri[k]);
                                }
                                --k;
                            }
                        }
                    }
                }
                click = true;
            }
            if (event->is<Event::MouseButtonReleased>())
            {
                click = false;
            }
        }
        bool bi_dang_bay = false;
        for (auto &x : ds_bi)
        {
            if (x.status != 0)
            {
                bi_dang_bay = true;
                break;
            }
        }
        if (!bi_dang_bay && !fswap.empty())
        {
            pair<int, int> p = fswap.front();
            fswap.erase(fswap.begin());

            Vector2f A = ds_bi[p.first].bi.getPosition();
            Vector2f B = ds_bi[p.second].bi.getPosition();

            ds_bi[p.first].dich = B;
            ds_bi[p.first].h = 550.f;
            ds_bi[p.first].status = 1;

            ds_bi[p.second].dich = A;
            ds_bi[p.second].h = 780.f;
            ds_bi[p.second].status = 1;
        }
        float speed = 600.f;
        for (auto &x : ds_bi)
        {
            if (x.status == 0)
            {
                continue;
            }
            Vector2f p = x.bi.getPosition();
            if (x.status == 1)
            {
                float dy = x.h - p.y;
                if (fabs(dy) > 5)
                    x.bi.move({0, (dy > 0 ? 1 : -1) * speed * dt});
                else
                    x.status = 2;
            }
            else if (x.status == 2)
            {
                float dx = x.dich.x - p.x;
                if (fabs(dx) > 5)
                    x.bi.move({(dx > 0 ? 1 : -1) * speed * dt, 0});
                else
                    x.status = 3;
            }
            else
            {
                float dy = x.dich.y - p.y;
                if (fabs(dy) > 5)
                    x.bi.move({0, (dy > 0 ? 1 : -1) * speed * dt});
                else
                {
                    x.bi.setPosition(x.dich);
                    x.status = 0;
                }
            }
        }
        Window.clear(Color(10, 17, 33));
        Window.draw(nen_chinh);
        Window.draw(title1);
        Window.draw(title2);
        Window.draw(nut);
        Window.draw(start);

        for (auto &x : o_chon_mau)
            Window.draw(x);
        for (auto &x : o_them_bi)
            Window.draw(x);
        for (auto &x : ds_bi)
            Window.draw(x.bi);

        Window.display();
    }
}