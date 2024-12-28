/** Multiple peers Wireguard calculation.

Wireguard does not support overlapping allowed ips for multiple peers. To
cover all possible ips excluding others for secondary peers some
calculation is needed to mount the allowed ips list derived from 0.0.0.0.

@author Wanderley Caloni <wanderley.caloni@gmail.com>
@date 2024-08
*/

using System.Net;

public class Program
{
    private static void CalculateWireguardAllowedIps(ref HashSet<IPNetwork2> list, IPNetwork2 ip, IPNetwork2 remove)
    {
        if (ip.Cidr == 32)
        {
            if (ip != remove)
            {
                list.Add(ip);
            }
            else
            {
                Console.WriteLine("CalculateWireguardAllowedIps: found and skipping" + ip.ToString());
            }
        }
        else
        {
            HashSet<IPNetwork2> split = ip.Subnet((byte)(ip.Cidr + 1)).ToHashSet(); // [0]=left, [1]=right
            if (split.Count == 2)
            {
                List<IPNetwork2> splitList = split.ToList();
                if (splitList[0].Overlap(remove))
                {
                    list.Add(splitList[1]);
                    CalculateWireguardAllowedIps(ref list, splitList[0], remove);
                }
                else
                {
                    list.Add(splitList[0]);
                    CalculateWireguardAllowedIps(ref list, splitList[1], remove);
                }
            }
        }
    }
    private static void CalculateWireguardAllowedIps(ref HashSet<IPNetwork2> ipsToAdd, ref HashSet<IPNetwork2> ipsToRemove, ref HashSet<IPNetwork2> list)
    {
        foreach (IPNetwork2 remove in ipsToRemove)
        {
            var localIpsToAdd = new HashSet<IPNetwork2>(ipsToAdd);

            foreach (IPNetwork2 add in localIpsToAdd)
            {
                if (add.Overlap(remove))
                {
                    CalculateWireguardAllowedIps(ref list, add, remove);
                }
                else
                {
                    list.Add(add);
                }
            }
            ipsToAdd = list;
        }
    }
    private static void AddAllowedIp(ref HashSet<IPNetwork2> allowedIps, IPNetwork2 addThisIp, ref HashSet<IPNetwork2> notAddTheseIps)
    {
        if (addThisIp.Cidr == 32)
        {
            if( ! notAddTheseIps.Contains(addThisIp))
            {
                allowedIps.Add(addThisIp);
            }
            else
            {
                Console.WriteLine("AddAllowedIp: found and skipping " + addThisIp.ToString());
            }
        }
        else
        {
            List<IPNetwork2> split = addThisIp.Subnet((byte)(addThisIp.Cidr + 1)).ToList(); // [0]=left, [1]=right
            if (split.Count == 2)
            {
                bool overlapLeft = false, overlapRight = false;

                foreach(IPNetwork2 notIp in notAddTheseIps)
                {
                    if (split[0].Overlap(notIp))
                    {
                        overlapLeft = true;
                    }

                    if (split[1].Overlap(notIp))
                    {
                        overlapRight = true;
                    }

                    if( overlapLeft && overlapRight)
                    {
                        break;
                    }
                }

                if (overlapLeft)
                {
                    AddAllowedIp(ref allowedIps, split[0], ref notAddTheseIps);
                }
                else
                {
                    allowedIps.Add(split[0]);
                }

                if (overlapRight)
                {
                    AddAllowedIp(ref allowedIps, split[1], ref notAddTheseIps);
                }
                else
                {
                    allowedIps.Add(split[1]);
                }
            }
        }
    }

    public static int Main(string[] args)
    {
        HashSet<IPNetwork2> twoPeersHashSet = new HashSet<IPNetwork2>();
        IPNetwork2 allAllowedIPs = new IPNetwork2(IPAddress.Parse("0.0.0.0"), 0);
        IPNetwork2 removeIP = new IPNetwork2(IPAddress.Parse("66.22.190.0"), 32);
        CalculateWireguardAllowedIps(ref twoPeersHashSet, allAllowedIPs, removeIP);

        HashSet<IPNetwork2> multiplePeersHashSet = new HashSet<IPNetwork2>();
        HashSet<IPNetwork2> ipsToRemove = new HashSet<IPNetwork2>();
        //ipsToRemove.Add(new IPNetwork2(IPAddress.Parse("66.22.190.0"), 32));
        ipsToRemove.Add(new IPNetwork2(IPAddress.Parse("66.22.188.1"), 32));
        //ipsToRemove.Add(new IPNetwork2(IPAddress.Parse("66.22.190.1"), 32));
        //ipsToRemove.Add(new IPNetwork2(IPAddress.Parse("66.22.188.0"), 32));
        AddAllowedIp(ref multiplePeersHashSet, allAllowedIPs, ref ipsToRemove);

        var inMultipleExceptTwoPeers = multiplePeersHashSet.Except(twoPeersHashSet).ToHashSet();
        var inTwoPeersExceptMultiple = twoPeersHashSet.Except(multiplePeersHashSet).ToHashSet();

        Console.WriteLine("In multiple peers except two peers:");
        foreach (var ip in inMultipleExceptTwoPeers)
        {
            string s = "ips in " + ip.ToString() + ": ";
            foreach(var ip2 in ip.ListIPAddress() )
            {
                s += ip2.ToString() + " ";
            }
            Console.WriteLine(s);
        }

        Console.WriteLine("In two peers except multiple peers:");
        foreach (var ip in inTwoPeersExceptMultiple)
        {
            string s = "ips in " + ip.ToString() + ": ";
            foreach(var ip2 in ip.ListIPAddress() )
            {
                s += ip2.ToString() + " ";
            }
            Console.WriteLine(s);
        }

        return 0;
    }

}
