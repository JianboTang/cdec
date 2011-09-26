#ifndef _DTRAIN_SCORE_H_
#define _DTRAIN_SCORE_H_

#include "kbestget.h"

using namespace std;

namespace dtrain
{


struct NgramCounts
{
  unsigned N_;
  map<unsigned, unsigned> clipped;
  map<unsigned, unsigned> sum;

  NgramCounts(const unsigned N) : N_(N) { Zero(); } 

  void
  operator+=(const NgramCounts& rhs)
  {
    assert(N_ == rhs.N_);
    for (unsigned i = 0; i < N_; i++) {
      this->clipped[i] += rhs.clipped.find(i)->second;
      this->sum[i] += rhs.sum.find(i)->second;
    }
  }

  const NgramCounts
  operator+(const NgramCounts &other) const
  {
    NgramCounts result = *this;
    result += other;
    return result;
  }

  void
  Add(unsigned count, unsigned ref_count, unsigned i)
  {
    assert(i < N_);
    if (count > ref_count) {
      clipped[i] += ref_count;
    } else {
      clipped[i] += count;
    }
    sum[i] += count;
  }

  void
  Zero()
  {
    unsigned i;
    for (i = 0; i < N_; i++) {
      clipped[i] = 0;
      sum[i] = 0;
    }
  }

  void
  Print()
  {
    for (unsigned i = 0; i < N_; i++) {
      cout << i+1 << "grams (clipped):\t" << clipped[i] << endl;
      cout << i+1 << "grams:\t\t\t" << sum[i] << endl;
    }
  }
};

typedef map<vector<WordID>, unsigned> Ngrams;

inline Ngrams
make_ngrams(const vector<WordID>& s, const unsigned N)
{
  Ngrams ngrams;
  vector<WordID> ng;
  for (size_t i = 0; i < s.size(); i++) {
    ng.clear();
    for (unsigned j = i; j < min(i+N, s.size()); j++) {
      ng.push_back(s[j]);
      ngrams[ng]++;
    }
  }
  return ngrams;
}

inline NgramCounts
make_ngram_counts(const vector<WordID>& hyp, const vector<WordID>& ref, const unsigned N)
{
  Ngrams hyp_ngrams = make_ngrams(hyp, N);
  Ngrams ref_ngrams = make_ngrams(ref, N);
  NgramCounts counts(N);
  Ngrams::iterator it;
  Ngrams::iterator ti;
  for (it = hyp_ngrams.begin(); it != hyp_ngrams.end(); it++) {
    ti = ref_ngrams.find(it->first);
    if (ti != ref_ngrams.end()) {
      counts.Add(it->second, ti->second, it->first.size() - 1);
    } else {
      counts.Add(it->second, 0, it->first.size() - 1);
    }
  }
  return counts;
}

struct BleuScorer : public LocalScorer
{
  score_t Bleu(NgramCounts& counts, const unsigned hyp_len, const unsigned ref_len);
  score_t Score(vector<WordID>& hyp, vector<WordID>& ref_ids);
};

struct StupidBleuScorer : public LocalScorer
{
  score_t Score(vector<WordID>& hyp, vector<WordID>& ref);
};

struct SmoothBleuScorer : public LocalScorer
{
  score_t Score(vector<WordID>& hyp, vector<WordID>& ref);
};

// FIXME
/*struct ApproxBleuScorer : public LocalScorer
{
  bool prepped;

  NgramCounts* glob_onebest_counts;
  unsigned glob_hyp_len, glob_ref_len;

  void Prep(NgramCounts& counts, const unsigned hyp_len, const unsigned ref_len);
  void Reset();
  score_t Score(ScoredHyp& hyp, vector<WordID>& ref_ids, unsigned id);

  ApproxBleuScorer() 
  {
    glob_onebest_counts.Zero();
    glob_hyp_len = 0;
    glob_ref_len = 0;
  }
};*/


} // namespace

#endif

