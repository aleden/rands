#include <array>
#include <bitset>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

static const array<char, 26> chrs = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

static ifstream rnd_ifs;
static vector<bool> bits;

static bitset<5> random_5_bits() {
  if (bits.size() >= 5) {
    bitset<5> res;
    res[0] = bits[bits.size() - 5];
    res[1] = bits[bits.size() - 4];
    res[2] = bits[bits.size() - 3];
    res[3] = bits[bits.size() - 2];
    res[4] = bits[bits.size() - 1];

    bits.pop_back();
    bits.pop_back();
    bits.pop_back();
    bits.pop_back();
    bits.pop_back();

    return res;
  } else {
    char rnd_byte;
    rnd_ifs.read(&rnd_byte, 1);

    bitset<8> res(static_cast<unsigned long long>(rnd_byte));

    for (unsigned i = 0; i < res.size(); ++i)
      bits.push_back(res[i]);

    return random_5_bits();
  }
}

static char random_character() {
  unsigned long x;

  // 5 bits provides 32 possibilities, but we map to a smaller set. hence the
  // try, discard
  do
    x = random_5_bits().to_ulong();
  while (x >= chrs.size());

  return chrs[x];
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "usage: rands N" << endl;
    return 1;
  }

  unsigned len;
  {
    istringstream iss(argv[1]);
    iss >> len;
  }

  rnd_ifs.open("/dev/random", ios_base::in);
  if (!rnd_ifs) {
    cerr << "error: could not open /dev/random" << endl;
    return 1;
  }

  string res;
  res.reserve(len);

  for (unsigned i = 0; i < len; ++i)
    res.push_back(random_character());

  cout << res << '\n';

  return 0;
}
