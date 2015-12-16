

#ifndef _DEBUG_PRINTER_H_
#define _DEBUG_PRINTER_H_

typedef enum DebugLevel
{
  LVL_OFF,
  LVL_MIN_VERB,
  LVL_MID_VERB,
  LVL_MAX_VERB
} DebugLevel;

class DebugPrinter
{
public:
  DebugPrinter(void);
  DebugPrinter(DebugLevel level);
  ~DebugPrinter(void);

  void dprintf(DebugLevel level, const char *format, ...);
  void setLevel(DebugLevel level) throw(int);
  DebugLevel getLevel(void) const;
  bool isValidLevel(DebugLevel level) const;

private:
  DebugLevel level;
};


#endif /* _DEBUG_PRINTER_H_ */
