#include "gps_logger.h"

GpsLogger::GpsLogger(int id_, string device)
{
  m_Device = device;

  m_FName = "gps";
  m_Mode = MODE_PORT;
  m_Kill = false;
  m_LogginEnabled = false;
  m_Running = false;
  m_StateChanged = false;

  id = id_;

  m_Thread = new thread(&GpsLogger::Run, this);
}

void GpsLogger::SetOutFName(const string &fname)
{
  m_FName = fname;
}

void GpsLogger::SetMode(int mode)
{
  m_Mode = mode;
}

void GpsLogger::SetOutputFolder(const string &folder)
{
  m_Folder = folder;
}

void GpsLogger::SetHeader(const string &header)
{
  m_Header = header;
}

void GpsLogger::SetCallback(void (*clbk)(int, string))
{
  m_OnNewLine = clbk;
}
void GpsLogger::SetCallback(std::function<void(int, string)> clbk)
{
  m_OnNewLine = clbk;
}

void GpsLogger::StartLogging()
{
  unique_lock<mutex> lck(logger_mtx);

  CONSOLE.Log("GPS", id, "Start Logging");

  m_GPS = new std::ofstream(m_Folder + "/" + m_FName + ".log");

  if (!m_GPS->is_open())
    CONSOLE.LogError("GPS", id, "Error opening .log file", m_Folder + "/" + m_FName + ".log");

  if (m_Header != "")
    (*m_GPS) << m_Header << endl;

  stat.Duration_seconds = GetTimestamp();
  stat.Messages = 0;

  m_LogginEnabled = true;
  m_Running = true;
  m_StateChanged = true;
  cv.notify_all();
  CONSOLE.Log("GPS", id, "Done");
}

void GpsLogger::StopLogging()
{
  unique_lock<mutex> lck(logger_mtx);

  stat.Duration_seconds = GetTimestamp() - stat.Duration_seconds;
  m_StateChanged = false;
  m_GPS->close();
  delete m_GPS;
  SaveStat();

  m_LogginEnabled = false;
  m_Running = false;
  m_StateChanged = true;
  cv.notify_all();
}

void GpsLogger::Start()
{
  m_Running = true;
  m_StateChanged = true;
  cv.notify_all();
}

void GpsLogger::Stop()
{
  m_Running = false;
  m_StateChanged = true;
  cv.notify_all();
}

void GpsLogger::Kill()
{
  m_Kill = true;
  m_Running = false;
  m_StateChanged = true;
  cv.notify_all();
}

void GpsLogger::WaitForEnd()
{
  cv.notify_all();
  m_Thread->join();
}

int GpsLogger::OpenDevice()
{
  CONSOLE.Log("GPS", id, "Opening device mode:", m_Mode);
  if (m_Serial == nullptr)
    m_Serial = new serial(m_Device);

  int ret = 0;
  if (m_Mode == MODE_PORT)
    ret = m_Serial->open_port();
  else
    ret = m_Serial->open_file();

  if (ret < 0)
  {
    CONSOLE.LogError("GPS", id, "Failed opening", m_Device);
    return 0;
  }
  else
  {
    CONSOLE.Log("GPS", id, "Opened ", m_Device);
    return 1;
  }
}

void GpsLogger::Run()
{
  string line = "", buff = "";
  int line_fail_count = 0;
  int file_fail_count = 0;

  CONSOLE.Log("GPS", id, "Run thread");

  while (!m_Kill)
  {
    unique_lock<mutex> lck(mtx);
    while (!m_Running && !m_Kill)
      cv.wait(lck);

    m_StateChanged = false;
    if (m_Kill)
      break;

    if (m_Running)
    {
      if (!OpenDevice())
      {
        m_Running = false;
        continue;
      }
    }

    m_StateChanged = false;
    int blank_lines_count = 0;
    CONSOLE.Log("GPS", id, "Running");
    while (m_Running)
    {
      try
      {
        while (true)
        {
          static char c;
          c = m_Serial->get_char();
          if (c == '$')
          {
            line = buff;
            buff = "$";
            break;
          }
          buff += c;
          if (buff[buff.size() - 2] == '\01' || buff[buff.size() - 1] == '\07')
          {
            line = buff;
            buff = "\01\07";
            break;
          }
        }
      }
      catch (std::exception e)
      {
        CONSOLE.LogError("GPS", id, "Error", e.what());
        line_fail_count++;
        if (line_fail_count >= 20)
        {
          CONSOLE.LogError("GPS", id, "Failed 20 times readline");
          m_Running = false;
          line_fail_count = 0;
          break;
        }
        continue;
      }
      // When receiving continuously blank lines, sleep a bit
      if (line == "$")
      {
        blank_lines_count++;
        if (blank_lines_count > 10)
          usleep(1000);
        continue;
      }
      else
      {
        blank_lines_count = 0;
      }

      // Callback on new line
      if (m_OnNewLine)
        m_OnNewLine(id, line);

      if (m_StateChanged)
        break;

      if (m_LogginEnabled)
      {
        unique_lock<mutex> lck(logger_mtx);
        try
        {
          if (line.find("\n") == string::npos)
            line += "\n";
          if (m_GPS->is_open())
            (*m_GPS) << line << flush;
        }
        catch (std::exception e)
        {
          CONSOLE.LogError("GPS", id, "Error writing to file:", e.what());
          file_fail_count++;
          if (file_fail_count >= 20)
          {
            CONSOLE.LogError("GPS", id, "Failed 20 times writing to file");
            m_Running = false;
            file_fail_count = 0;
            break;
          }
        }
        stat.Messages++;
      }
    }

    if (!m_Running)
    {
      CONSOLE.Log("GPS", id, "closing port");
      m_Serial->close_port();
      CONSOLE.Log("GPS", id, "Done");
    }
  }
}

double GpsLogger::GetTimestamp()
{
  return duration_cast<duration<double, milli>>(system_clock::now().time_since_epoch()).count() / 1000;
}

void GpsLogger::SaveStat()
{
  if (stat.Messages == 0 || !path_exists(m_Folder))
    return;
  CONSOLE.Log("GPS", id, "Saving stat");
  stat.Average_Frequency_Hz = float(stat.Messages) / stat.Duration_seconds;
  SaveStruct(stat, m_Folder + "/" + m_FName + ".json");
  CONSOLE.Log("GPS", id, "Done");
}
