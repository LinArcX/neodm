#ifndef SW_ACTIVE_H
#define SW_ACTIVE_H

#include <launcher.hpp>

#include <aria2/aria2.h>
#include <chrono>
#include <mutex>
#include <ncurses.h>
#include <queue>
#include <string>
#include <thread>

class SwActive;

// Interface to send message to downloader thread from UI thread
struct Job {
    virtual ~Job() {};
    virtual void execute(aria2::Session* session) = 0;
};

// Interface to report back to UI thread from downloader thread
struct Notification {
    virtual ~Notification() {};
    virtual void notify(SwActive* frame) = 0;
};

// std::queue<T> wrapper synchronized by mutex. In this example
// program, only one thread consumes from the queue, so separating
// empty() and pop() is not a problem.
template <typename T>
class SynchronizedQueue {
public:
    SynchronizedQueue() {}
    ~SynchronizedQueue() {}
    void push(std::unique_ptr<T>&& t)
    {
        std::lock_guard<std::mutex> l(m_);
        q_.push(std::move(t));
    }
    std::unique_ptr<T> pop()
    {
        std::lock_guard<std::mutex> l(m_);
        std::unique_ptr<T> t = std::move(q_.front());
        q_.pop();
        return t;
    }
    bool empty()
    {
        std::lock_guard<std::mutex> l(m_);
        return q_.empty();
    }

private:
    std::queue<std::unique_ptr<T>> q_;
    std::mutex m_;
};

typedef SynchronizedQueue<Job> JobQueue;
typedef SynchronizedQueue<Notification> NotifyQueue;

// Job to shutdown downloader thread
struct ShutdownJob : public Job {
    ShutdownJob(bool force)
        : force(force)
    {
    }
    virtual void execute(aria2::Session* session)
    {
        aria2::shutdown(session, force);
    }
    bool force;
};

// Job to send URI to download and options to downloader thread
struct AddUriJob : public Job {
    AddUriJob(std::vector<std::string>&& uris, aria2::KeyVals&& options)
        : uris(uris)
        , options(options)
    {
    }
    virtual void execute(aria2::Session* session)
    {
        // TODO check return value
        aria2::addUri(session, 0, uris, options);
    }
    std::vector<std::string> uris;
    aria2::KeyVals options;
};

// This struct is used to report download progress for active
// downloads from downloader thread to UI thread.
struct DownloadStatus {
    aria2::A2Gid gid;
    int64_t totalLength;
    int64_t completedLength;
    int downloadSpeed;
    int uploadSpeed;
    std::string filename;
};

class SwActive {
private:
    JobQueue jobq_;
    NotifyQueue notifyq_;
    std::thread downloaderThread_;

    time_t start, end;

    std::shared_ptr<Launcher> _launcher;

    int m_current_line = 0; // line number within m_nPageLines of current selection (0 thru (m_nPageSize - 1))
    int m_current_char = 0; // value of currently selected character

    int PAGE_SIZE; // fixed view of 16 line items per "page"
    const int START_Y = 3;
    const int PAGE_TOP = 0; // first line number on the page of line items
    const int FIRST_CHAR = 0;

    int x_first_col;
    int x_second_col;
    int x_third_col;
    int x_forth_col;
    int x_fifth_col;
    int x_sixth_col;
    int x_seventh_col;
    int x_eighth_col;

    int col_third_length;
    int col_forth_length;
    int col_fifth_length;
    int col_sixth_length;
    int col_seventh_length;
    int col_eighth_length;

    void show_popup(std::string val);
    void drawStaticItems();

public:
    SwActive(std::shared_ptr<Launcher> launcher);
    virtual ~SwActive();

    void drawDynamicItems();
    int show_active_window();
    void spawn_new_download(int m_current_char);
    void UpdateActiveStatus(const std::vector<DownloadStatus>& v);
    void prevent_downloaded_files(int index);
};

struct DownloadStatusNotification : public Notification {
    DownloadStatusNotification(std::vector<DownloadStatus>&& v)
        : v(v)
    {
    }
    virtual void notify(SwActive* frame)
    {
        frame->UpdateActiveStatus(v);
    }
    std::vector<DownloadStatus> v;
};

struct ShutdownNotification : public Notification {
    ShutdownNotification() {}
    virtual void notify(SwActive* frame)
    {
        //frame->Close();
    }
};

#endif /* SW_ACTIVE_H */
