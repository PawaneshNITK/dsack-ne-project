/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 NITK Surathkal
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Shikha Bakshi <shikhabakshi912@gmail.com>
 *          Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 */
#include "ns3/log.h"
#include "ns3/tcp-westwood-plus.h"
#include "tcp-general-test.h"
#include "ns3/simple-channel.h"
#include "ns3/node.h"
#include "tcp-error-model.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpDsackTest");

class TcpDsackSackTest : public TcpGeneralTest
{
public:
  /**
   * \brief Constructor
   * \param congControl Type of congestion control.
   * \param seqToKill Sequence number of the packet to drop.
   * \param dsack Boolean variable to enable or disable DSACK.
   * \param msg Test message.
   */
  TcpDsackSackTest (bool sack, bool dsack, const std::string &msg);

  virtual Ptr<TcpSocketMsgBase> CreateSenderSocket (Ptr<Node> node);

  protected:

  virtual void Tx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who);

  bool m_sackState;
  bool m_dsackState;

};

TcpDsackSackTest::TcpDsackSackTest (bool sack, bool dsack, const std::string &msg)
  : TcpGeneralTest (msg),
    m_sackState (sack),
    m_dsackState (dsack)
{
}

Ptr<TcpSocketMsgBase>
TcpDsackSackTest::CreateSenderSocket (Ptr<Node> node)
{
  Ptr<TcpSocketMsgBase> socket = TcpGeneralTest::CreateSenderSocket (node);
  socket->SetAttribute ("Dsack", BooleanValue (m_dsackState));
  socket->SetAttribute ("Sack", BooleanValue (m_sackState));
  return socket;
}

void
TcpDsackSackTest::Tx (const Ptr<const Packet> p, const TcpHeader&h, SocketWho who)
{

  if ((h.GetFlags () & TcpHeader::SYN))
    {
      std::cout << h.HasOption (TcpOption::SACKPERMITTED);
      NS_TEST_ASSERT_MSG_EQ (h.HasOption (TcpOption::SACKPERMITTED), true,
                             "SackPermitted disabled but option enabled");
    }
}

/**
 * \ingroup internet-test
 * \ingroup tests
 *
 * \brief Testsuite for the DSACK
 */
class TcpDsackTestSuite : public TestSuite
{
public:
  TcpDsackTestSuite () : TestSuite ("tcp-dsack-test", UNIT)
  {
    AddTestCase (new TcpDsackSackTest (true, true, "Sack enable"), TestCase::QUICK);
    AddTestCase (new TcpDsackSackTest (false, true, "Sack disable"), TestCase::QUICK);
  }
};

static TcpDsackTestSuite g_TcpDsackTestSuite; //!< Static variable for test initialization
