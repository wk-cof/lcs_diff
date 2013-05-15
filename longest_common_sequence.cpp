#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


class LCS
{
public:
  LCS(string str1, string str2);
  void Calculate();
  void GetLCS(bool upmost);

private:
  void CalculateLCS();
  void addLeftElement();

  string m_str1;
  string m_str2;
  int startIdx;
  int endIdx1, endIdx2;

  enum direction{invalid, left, up, upleft, up_and_left};
  struct elem
  {
    int len;        // current length of the longest sequence
    direction dir;  // direction to follow
  };
  vector<vector<elem> > m_lcsMatrix;
};



void LCS::GetLCS(bool upmost)
{
  int i = endIdx1-1, j = endIdx2 - 1;
  int strSize1 = m_str1.size();
  int strSize2 = m_str2.size();
  vector<string> longestCommonSubsequence; // parts that are in both str1 and str2 
  vector<string> addedSequences;    // parts of the input that are in str2 and aren't in str1
  vector<string> deletedSequences;  // parts of the input that are in str1 and aren't in str2
  string curWord(' ');

  direction currentDirection = m_lcsMatrix[i][j].dir;
  if (currentDirection == up_and_left)
    currentDirection = upmost? up : left;
  while (m_lcsMatrix[i][j].len > 0)
  {
    switch (m_lcsMatrix[i][j].dir)
    {
      case left:
        if (currentDirection == left)
          deletedSequences[deletedSequences.size()-1].append(m_str1[i]);

//        addLeftElement();
        j --;
        break;
      case up:
//        addUpElement();
        i--;
        break;
      case upleft:
        if (currentDirection == upleft)
          longestCommonSubsequence[longestCommonSubsequence.size()-1].append(m_str1[i]); 
        else
        {
          currentDirection = upleft;
          longestCommonSubsequence.push_back(m_str1[i]);
        }
//        addUpLeftElement();
        i--;
        j--;
        break;
      case up_and_left:
        if (upmost)
        {
//          addUpElement();
          i--;
        }
        else
        {
//          addLeftElement();
          j--;
        }
      case invalid: // this should never happen...
      defalut: 
        cerr << "LCS path is invalid. This should never happen"<<endl;
        return;

    }
  }
  return;
}

LCS::LCS(string str1, string str2) : 
  m_str1(str1), 
  m_str2(str2), 
  startIdx(0), 
  endIdx1(str1.size() - 1),
  endIdx2(str2.size() - 1)
{
}

void LCS::Calculate()
{ if (!m_str1.size() || !m_str2.size()) return; int startIdx = 0; // optimization, skip the beginning and end (since it's most likely is the same) while (startIdx <= endIdx1 && startIdx <= endIdx2 && m_str1[startIdx] == m_str2[startIdx]) startIdx++; 
  while (endIdx1 >= startIdx && endIdx2 >= startIdx && m_str1[endIdx1] == m_str2[endIdx2]) 
  { 
    endIdx1 --;
    endIdx2 --;
  }
  //lcsLength += startIdx-1 + m_str1.size()-endIdx1;
  CalculateLCS();
  return;
}

void LCS::CalculateLCS()
{
  m_lcsMatrix.resize(endIdx1-startIdx + 1);
  m_lcsMatrix[0].resize(endIdx12 - startIdx + 1, elem());
  int iShifted, jShifted;
  for (int i=startIdx; i <= endIdx1; ++i)
  {
    iShifted = i - startIdx + 1;
    m_lcsMatrix[iShifted].resize(endIdx2 - startIdx + 1);
    m_lcsMatrix[iShifted][0].len = 0;
    m_lcsMatrix[iShifted][0].dir = invalid;
    for(int j = startIdx; j <= endIdx2; ++j)
    {
      jShifted = j - startIdx + 1;

      if (m_str1[i] == m_str2[j])
      {
        if (j == 0 || i == 0) // only 1 element, nothing to fall back to
        {
          m_lcsMatrix[iShifted][jShifted].len = 1;
          m_lcsMatrix[iShifted][jShifted].dir = invalid;
        }
        else
        {
          m_lcsMatrix[iShifted][jShifted].len = 1+m_lcsMatrix[i-1][j-1].len;
          m_lcsMatrix[iShifted][jShifted].dir = upleft;
        }

      }
      else // not equal
      {
        if (j == 0 || i == 0) // only 1 element, nothing to fall back to
        {
          m_lcsMatrix[i][j].len = 0;
          m_lcsMatrix[i][j].dir = invalid;
        }
        else
        {
          if (m_lcsMatrix[i-1][j].len > m_lcsMatrix[i][j-1].len) // top is longer
          {
            m_lcsMatrix[i][j].len = m_lcsMatrix[i-1][j].len;
            m_lcsMatrix[i][j].dir = up;
          }
          else if (m_lcsMatrix[i-1][j].len < m_lcsMatrix[i][j-1].len) // left is longer
          {
            m_lcsMatrix[i][j].len = m_lcsMatrix[i][j-1].len;
            m_lcsMatrix[i][j].dir = left;
          }
          else // same length, leave both
          {
            m_lcsMatrix[i][j].len = m_lcsMatrix[i][j-1].len;
            m_lcsMatrix[i][j].dir = up_and_left; 
          }
        }
      }
    }

  }
  return;
}

int main(int argc, char *argv[])
{
  if (argc < 3 || !argv[1] || !argv[2])
  {
    cout << "Usage: lcs filename1 filename2"<<endl;
    return 0;
  }
  bool debug_mode(false);
  if (argc == 4)
    debug_mode = true;


  vector<string> strArr;
  strArr.resize(2,"");
  int i;
  fstream f;
  try
  {
    for(i=0; i < 2; i++)
    {
      string temp;
      f.open(argv[i+1], fstream::in);
      if (!f)
      {
        cerr << "Error opening file : " << argv[1] << endl;
        return 0;
      }
      f.seekg(0, std::ios::end);
      temp.resize(f.tellg());
      f.seekg(0, std::ios::beg);
      f.read(&temp[0], temp.size());
      strArr[i] = temp;
      f.close();
    }
  }
  catch(exception e)
  {
    cerr << "can't read file " + string(argv[i])<< endl;
  }

  if (debug_mode)
    cout << "***finding longest common sequence between: "<< endl<< strArr[0] << endl << endl << "***and : " << endl << strArr[1] << endl;

  LCS myLcs(strArr[0], strArr[1]);
  myLcs.Calculate();

  return 0;
}
