#include <iostream>
#include <cmath>

// ============================================================
// Vec: N-boyutlu vektör yapısı
// Heap üzerinde dinamik bellek kullanır, boyut runtime'da belirlenir.
// Strang - Linear Algebra çalışmalarına paralel geliştirilmektedir.
// ============================================================

struct Vec
{
public:
    int dim;      // Vektörün boyutu (kaç bileşen taşıdığı)
    float* data;  // Bileşenleri tutan heap adresi

    // Sadece boyut verilerek oluşturma: bileşenler başlatılmaz
    Vec(int dime)
    {
        dim = dime;
        data = new float[dim];
    }

    // Boyut ve hazır bir float dizisiyle oluşturma
    Vec(int dime, float* dat)
    {
        dim = dime;
        data = dat;
    }

    // Varsayılan constructor: boş vektör
    Vec() : dim(0), data(nullptr) {}

    // Atama operatörü: dim ve data pointer'ı kopyalar
    // NOT: shallow copy, iki Vec aynı belleği paylaşır.
    // Destructor eklenince deep copy'e geçilmeli.
    Vec operator=(const Vec& other)
    {
        this->dim = other.dim;
        this->data = other.data;
        return *this;
    }

    // Dot product (iç çarpım): v * w = v1w1 + v2w2 + ... + vnwn
    // Geometrik anlamı: iki vektörün ne kadar aynı yönde olduğunu ölçer.
    // Dik vektörlerde 0, aynı yönde maksimum çıkar.
    float operator*(const Vec& other)
    {
        float sum = 0;
        for (int i = 0; i < dim; i++)
        {
            sum += data[i] * other.data[i];
        }
        return sum;
    }

    // Vektör toplama: v + w
    // Boyutlar eşitse bileşen bileşen toplanır.
    // Boyutlar farklıysa küçük vektörün dışındaki bileşenler
    // büyük vektörden aynen alınır.
    Vec operator+(const Vec& other)
    {
        int smallestDim;
        Vec biggestVec;

        if (other.dim > dim)
        {
            smallestDim = dim;
            biggestVec = other;
        }
        else if (other.dim == dim)
        {
            // Eşit boyut: direkt topla
            float* newData = new float[dim];
            for (int i = 0; i < dim; i++)
            {
                newData[i] = other.data[i] + data[i];
            }
            Vec newVec(dim, newData);
            return newVec;
        }
        else
        {
            smallestDim = other.dim;
            biggestVec = *this;
        }

        // Farklı boyut: ortak kısım toplanır, kalan büyük vektörden alınır
        float* newData = new float[biggestVec.dim];
        for (int i = 0; i < smallestDim; i++)
        {
            newData[i] = other.data[i] + data[i];
        }
        for (int i = smallestDim; i < biggestVec.dim; i++)
        {
            newData[i] = biggestVec.data[i];
        }
        Vec newVec(biggestVec.dim, newData);
        return newVec;
    }

    // Vektör uzunluğu (L2 norm): ||v|| = sqrt(v1^2 + v2^2 + ... + vn^2)
    // Pisagor teoreminin N boyuta genellemesi.
    float length()
    {
        float sum = 0;
        for (int i = 0; i < dim; i++)
        {
            sum += data[i] * data[i];
        }
        return sqrt(sum);
    }

    // İki vektör arasındaki açının kosinüsü
    // Formül: cos(theta) = (v . w) / (||v|| * ||w||)
    // Sonuç -1 ile 1 arasında:
    //   1  -> aynı yön (0 derece)
    //   0  -> dik (90 derece)
    //  -1  -> zıt yön (180 derece)
    float cos(Vec& other)
    {
        float dotprod = *this * other;
        float len = this->length() * other.length();
        return dotprod / len;
    }
};

int main()
{
    float arr[] = { 1.0f, 2.0f };
    float arr2[] = { 2.0f, 3.0f };

    Vec v1(2, arr);
    Vec v2(2, arr2);

    // (1,2) ve (2,3) neredeyse aynı yönde, cos ~0.99 beklenir
    std::cout << "cos(v1, v2) = " << v1.cos(v2) << std::endl;

    return 0;
}