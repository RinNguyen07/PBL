#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace sf;
using namespace std;

// --- CẤU TRÚC DỮ LIỆU ---
struct Manhghep
{
    ConvexShape Manh;
    Color maugoc;
    float scale_hientai = 1.0f;
    float scale_Muctieu = 1.0f;
};

struct ButtonUI
{
    ConvexShape hinh;
    Text chu;
    float scale_hientai = 1.0f;
    float scale_Muctieu = 1.0f;

    ButtonUI(const Font &font, string noi_dung, unsigned int size)
        : chu(font, noi_dung, size) {}

    // Hàm update giờ chỉ tác động scale lên 'hinh', không tác động lên 'chu'
    void update(Vector2f mouse)
    {
        scale_Muctieu = (hinh.getGlobalBounds().contains(mouse)) ? 1.05f : 1.0f;
        scale_hientai += (scale_Muctieu - scale_hientai) * 0.15f;
        hinh.setScale({scale_hientai, scale_hientai});
        // chu.setScale({1.0f, 1.0f}); // Giữ nguyên chữ không scale
    }

    void draw(RenderWindow &window)
    {
        window.draw(hinh);
        window.draw(chu);
    }
};

// --- HÀM VẼ HÌNH CƠ BẢN ---
ConvexShape Tam_giac(Vector2f Vi_tri_dat, float radius, float g1, float g2, Color color)
{
    ConvexShape shape(3);
    shape.setPoint(0, {0, 0});
    shape.setPoint(1, {radius * std::cos(g1), radius * std::sin(g1)});
    shape.setPoint(2, {radius * std::cos(g2), radius * std::sin(g2)});
    shape.setFillColor(color);
    shape.setPosition(Vi_tri_dat);
    return shape;
}

ConvexShape HCN_bo_goc(Vector2f size, float radius, Color color, Vector2f position)
{
    const int Diem_ve = 20;
    ConvexShape shape(Diem_ve * 4);
    float pi = 3.141592654f;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < Diem_ve; j++)
        {
            float goc = (i * 90.f + j * (90.f / (Diem_ve - 1))) * pi / 180.f;
            Vector2f tam;
            if (i == 0)
                tam = {size.x - radius, size.y - radius};
            else if (i == 1)
                tam = {radius, size.y - radius};
            else if (i == 2)
                tam = {radius, radius};
            else
                tam = {size.x - radius, radius};
            shape.setPoint(i * Diem_ve + j, {tam.x + radius * std::cos(goc), tam.y + radius * std::sin(goc)});
        }
    }
    shape.setOrigin(size / 2.f);
    shape.setFillColor(color);
    shape.setPosition(position + size / 2.f);
    return shape;
}

// --- CÁC HÀM XỬ LÝ CHÍNH ---

void xuLySuKien(RenderWindow &window, int &trangThai, Vector2f mouse, Vector2f Tam, float radius,
                vector<Manhghep> &Lucgiac, vector<Color> &maudachon, vector<vector<Color>> &danhSachHoanVi,
                ButtonUI &btnReset, ButtonUI &btnCont, ButtonUI &btnBack, vector<ButtonUI> &dsKhung, const Font &font)
{

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
            window.close();

        if (const auto *mouseClick = event->getIf<Event::MouseButtonPressed>())
        {
            if (mouseClick->button == Mouse::Button::Left)
            {
                if (trangThai == 0)
                {
                    float dx = mouse.x - Tam.x, dy = mouse.y - Tam.y;
                    if (std::sqrt(dx * dx + dy * dy) <= radius && maudachon.size() < 3)
                    {
                        float angle = std::atan2(dy, dx);
                        if (angle < 0)
                            angle += 2 * 3.14159f;
                        int index = static_cast<int>(angle / (2 * 3.14159f / 6));
                        if (std::find(maudachon.begin(), maudachon.end(), Lucgiac[index].maugoc) == maudachon.end())
                            maudachon.push_back(Lucgiac[index].maugoc);
                    }
                    if (btnReset.hinh.getGlobalBounds().contains(mouse))
                        maudachon.clear();
                    if (btnCont.hinh.getGlobalBounds().contains(mouse) && maudachon.size() == 3)
                    {
                        danhSachHoanVi = {{maudachon[0], maudachon[1], maudachon[2]}, {maudachon[1], maudachon[2], maudachon[0]}, {maudachon[2], maudachon[0], maudachon[1]}};

                        dsKhung.clear();
                        for (int i = 0; i < 3; i++)
                        {
                            dsKhung.emplace_back(font, "Cach " + to_string(i + 1), 22);
                            dsKhung[i].hinh = HCN_bo_goc({700, 140}, 15, Color(50, 50, 50), {250, 180.f + i * 180.f});
                            // Đặt chữ lệch sang trái một chút trong khung
                            dsKhung[i].chu.setPosition(dsKhung[i].hinh.getPosition() - Vector2f(300, 15));
                        }
                        trangThai = 1;
                    }
                }
                else if (trangThai == 1 && btnBack.hinh.getGlobalBounds().contains(mouse))
                {
                    trangThai = 0;
                }
            }
        }
    }
}

void veManHinh1(RenderWindow &window, Vector2f mouse, Vector2f Tam, float radius,
                vector<Manhghep> &Lucgiac, const vector<Color> &maudachon, ButtonUI &btnReset, ButtonUI &btnCont, Text &ThongBao)
{

    float dx = mouse.x - Tam.x, dy = mouse.y - Tam.y;
    float dist = std::sqrt(dx * dx + dy * dy);
    float angle = std::atan2(dy, dx);
    if (angle < 0)
        angle += 2 * 3.14159f;

    for (int i = 0; i < 6; i++)
    {
        Lucgiac[i].scale_Muctieu = (dist <= radius && angle >= i * 2 * 3.14159f / 6 && angle <= (i + 1) * 2 * 3.14159f / 6) ? 1.15f : 1.0f;
        Lucgiac[i].scale_hientai += (Lucgiac[i].scale_Muctieu - Lucgiac[i].scale_hientai) * 0.1f;
        Lucgiac[i].Manh.setScale({Lucgiac[i].scale_hientai, Lucgiac[i].scale_hientai});
        window.draw(Lucgiac[i].Manh);
    }
    btnReset.update(mouse);
    btnReset.draw(window);
    btnCont.update(mouse);
    btnCont.draw(window);
    for (int i = 0; i < (int)maudachon.size(); i++)
    {
        window.draw(HCN_bo_goc({180, 80}, 15, maudachon[i], {900.f, 100.f + i * 120.f}));
    }
    window.draw(ThongBao);
}

void veManHinh2(RenderWindow &window, Vector2f mouse, ButtonUI &btnBack, Text &tieuDe,
                vector<ButtonUI> &dsKhung, const vector<vector<Color>> &danhSachHoanVi)
{

    btnBack.update(mouse);
    btnBack.draw(window);
    window.draw(tieuDe);

    for (int i = 0; i < (int)dsKhung.size(); i++)
    {
        dsKhung[i].update(mouse); // Chỉ cái khung HCN sẽ scale
        dsKhung[i].draw(window);  // Vẽ khung và chữ (chữ đã bị bỏ lệnh setScale nên sẽ đứng yên)

        for (int j = 0; j < 3; j++)
        {
            CircleShape bi(40);
            bi.setOrigin({40, 40});
            bi.setFillColor(danhSachHoanVi[i][j]);
            // Vị trí cố định theo tâm của khung, không phụ thuộc vào scale của khung
            bi.setPosition({dsKhung[i].hinh.getPosition().x - 100 + j * 150, dsKhung[i].hinh.getPosition().y});
            bi.setScale({1.0f, 1.0f}); // Ép scale luôn là 1.0
            window.draw(bi);
        }
    }
}

// --- MAIN ---
int main()
{
    RenderWindow Window(VideoMode({1200, 800}), "PBL - SFML 3.0 Fixed Scale");
    Window.setFramerateLimit(60);

    Font font;
    if (!font.openFromFile("ARIAL.TTF"))
        return -1;

    int trangThai = 0;
    Vector2f Tam({400, 400});
    float radius = 250;
    vector<Color> maudachon;
    vector<vector<Color>> danhSachHoanVi;
    vector<ButtonUI> dsKhung;

    vector<Color> dsmau = {Color::Red, Color::Green, Color::Blue, Color::Yellow, Color::Magenta, Color::Cyan};
    vector<Manhghep> Lucgiac;
    for (int i = 0; i < 6; i++)
    {
        Manhghep tmp;
        tmp.maugoc = dsmau[i];
        tmp.Manh = Tam_giac(Tam, radius, i * 2 * 3.14159f / 6, (i + 1) * 2 * 3.14159f / 6, dsmau[i]);
        Lucgiac.push_back(tmp);
    }

    ButtonUI btnReset(font, "Reset", 20), btnCont(font, "Continue", 20), btnBack(font, "Back", 20);

    btnReset.hinh = HCN_bo_goc({150, 50}, 10, Color(150, 0, 0), {800, 650});
    btnReset.chu.setOrigin(btnReset.chu.getLocalBounds().getCenter());
    btnReset.chu.setPosition(btnReset.hinh.getPosition());

    btnCont.hinh = HCN_bo_goc({150, 50}, 10, Color(0, 150, 0), {1000, 650});
    btnCont.chu.setOrigin(btnCont.chu.getLocalBounds().getCenter());
    btnCont.chu.setPosition(btnCont.hinh.getPosition());

    btnBack.hinh = HCN_bo_goc({120, 45}, 10, Color(100, 100, 100), {30, 30});
    btnBack.chu.setOrigin(btnBack.chu.getLocalBounds().getCenter());
    btnBack.chu.setPosition(btnBack.hinh.getPosition());

    Text ThongBao(font, "Hay chon 3 mau de tiep tuc", 30);
    ThongBao.setPosition({50, 50});
    Text tieuDe(font, "3 CACH HOAN VI MAU DA CHON", 35);
    tieuDe.setPosition({320, 50});

    while (Window.isOpen())
    {
        Vector2f mouse = Window.mapPixelToCoords(Mouse::getPosition(Window));
        xuLySuKien(Window, trangThai, mouse, Tam, radius, Lucgiac, maudachon, danhSachHoanVi, btnReset, btnCont, btnBack, dsKhung, font);

        Window.clear(Color(30, 30, 30));
        if (trangThai == 0)
            veManHinh1(Window, mouse, Tam, radius, Lucgiac, maudachon, btnReset, btnCont, ThongBao);
        else
            veManHinh2(Window, mouse, btnBack, tieuDe, dsKhung, danhSachHoanVi);
        Window.display();
    }
    return 0;
}